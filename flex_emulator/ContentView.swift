/*
	CONTENTVIEW.SWIFT
	-----------------
	Copyright (c) 2021 Andrew Trotman
*/
import SwiftUI
import Foundation

class image_changer: ObservableObject
	{
	var counter = 0

	let offscreen_bitmap = CGContext(data: malloc(480 * 240 * 4), width: 480, height: 240, bitsPerComponent: 8, bytesPerRow: 480 * 4, space: CGColorSpaceCreateDeviceRGB(), bitmapInfo: CGImageAlphaInfo.noneSkipLast.rawValue)!
	@Published public var image = UIImage(named: "480x240")!
	public var frame_buffer: UnsafeMutablePointer<UInt8>?

	init()
		{
		/* Nothing */
		}
	}

class machine_changer: ObservableObject
	{
	@Published public var pointer: UnsafeRawPointer? = nil

	init()
		{
		/* Nothing */
		}
	}

/*
	STRUCT CONTENTVIEW
	------------------
*/
struct ContentView: View
	{
	@Environment(\.scenePhase) var scene_phase

	@State var flash_state = false	// Should the cursor be in the visible (or the hidden blink state)?

	let CPU_speed:Int64 = 2000000			// 1,000,000 is 1 MHz
	let iOS_timer_hz:Int64 = 25		// interrupts per second

	@State var flash_timer = Timer.publish(every: 0.5, on: .main, in: .common).autoconnect()
	@State var cpu_timer = Timer.publish(every: 1.0/25.0, on: .main, in: .common).autoconnect()

	@State var machine = machine_changer()
	@State var paused = false

	@State var screen = terminal()
	@State var keypad = keyboard()

	@StateObject var img_screen = image_changer()

	/*
		INIT()
		------
	*/
	init()
		{
		reset()
		}

	/*
		RESET()
		-------
	*/
	func reset()
		{
		paused = false;
		flash_state = false
		machine_reset(machine.pointer);
		screen.reset()
		keypad.reset()
		}

	/*
		FRAME_SIZE()
		------------
	*/
	func frame_size() -> CGFloat
		{
		return 3
		}

	/*
		BODY VIEW
		---------
	*/
	var body: some View
		{
		VStack
			{
			Spacer().frame(maxHeight: frame_size()).layoutPriority(1)

			Image(uiImage: img_screen.image)
				.resizable()
				.frame(width:UIScreen.main.bounds.size.width - frame_size(), height:UIScreen.main.bounds.size.width - frame_size())
				.onAppear(perform:
					{
					img_screen.frame_buffer = img_screen.offscreen_bitmap.data!.assumingMemoryBound(to: UInt8.self)
					render_text_screen()
					})

			Spacer().frame(idealHeight: frame_size()).layoutPriority(-1)

			Group
				{
				let keyboard_image_to_use = keypad.keyboard_image()

				GeometryReader
					{ (geometry) in
					VStack
						{
						Divider().frame(minHeight: frame_size(), alignment:.bottom).background(Color.black)
						Image(uiImage: keyboard_image_to_use)
							.resizable()
							.frame(width: keypad.width(size: geometry.size), height: keypad.height(size: geometry.size), alignment: .bottom)
							.simultaneousGesture(
								DragGesture(minimumDistance: 0, coordinateSpace: .local).onEnded
									{
									let ascii = keypad.keyboard_press(width: keypad.width(size: geometry.size), height: keypad.height(size: geometry.size), location: $0.location)
									switch (ascii)
										{
										case keyboard.key.KEY_NONE.rawValue:
											break
										case keyboard.key.KEY_PAUSE.rawValue:
											if (!paused)
												{
												flash_timer.upstream.connect().cancel()
												cpu_timer.upstream.connect().cancel()
												}
											else
												{
												cpu_timer = Timer.publish(every: 1.0/25.0, on: .main, in: .common).autoconnect()
												flash_timer = Timer.publish(every: 0.5, on: .main, in: .common).autoconnect()
												}
											paused = !paused
											/*
												Turn the cursor on and render the screen state as it is.
											*/
											flash_state = true
											render_text_screen()
										case keyboard.key.KEY_40_80.rawValue:		// 40 / 80 Column switch
											screen.set_width(new_width: screen.get_width() == .eighty ? .fourty : .eighty)
											render_text_screen()
										case keyboard.key.KEY_RESET.rawValue:		// Reset button
											reset();
										default:
											machine_queue_key_press(machine.pointer, CChar(ascii))
										}
									}
							)
						}
    					.frame(width: geometry.size.width, height: geometry.size.height, alignment: .bottom)
					}
				}

			Spacer()
				.frame(maxHeight: frame_size())
				.onReceive(cpu_timer)
					{ _ in
					if (machine.pointer != nil)
						{
						if (!paused)
							{
							var screen_did_change = false

							let end_cycle = machine_cycles_spent(machine.pointer) + CPU_speed / iOS_timer_hz

							while (machine_cycles_spent(machine.pointer) < end_cycle)
								{
								for _ in 0 ... Int(CPU_speed / iOS_timer_hz / 10)
									{
									machine_step(machine.pointer);
									}

								var response = machine_dequeue_serial_output(machine.pointer)
								while (response <= 0xFF)
									{
									screen.print_character(raw_character: UInt8(response & 0xFF))
									screen_did_change = true
									response = machine_dequeue_serial_output(machine.pointer)
									}

								if screen_did_change
									{
									render_text_screen()
									break;
									}
								}
							}
						}
					}
				.onReceive(flash_timer)
					{ _ in
					if !paused
						{
						flash_state = !flash_state
						render_text_screen()
						}
					}
				.onAppear
					{
					if (machine.pointer == nil)
						{
						machine.pointer = machine_construct()
						machine_deserialise(machine.pointer)
						deserialise(path: get_serialised_filename())
						}
					}
				.onChange(of: scene_phase)
					{ new_phase in
					switch new_phase
						{
						case .active:
							machine_deserialise(machine.pointer)
							deserialise(path: get_serialised_filename())
						case .inactive, .background:
							machine_serialise(machine.pointer)
							serialise(path: get_serialised_filename())
						@unknown default:
							(
							/* Nothing */
							)
						}
					}
		}
	.background(Color.white)
	}

	/*
		GET_SERIALISED_FILENAME()
		-------------------------
	*/
	func get_serialised_filename() -> URL
		{
		return FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)[0].appendingPathComponent("terminal.state")
		}

	/*
		SERIALISE()
		-----------
	*/
	func serialise(path: URL)
		{
		do
			{
			FileManager.default.createFile(atPath: path.path, contents: nil, attributes: nil)

			let file = try FileHandle(forWritingTo: path)
			try file.seekToEnd()

			screen.serialise(file: file)
			keypad.serialise(file: file)

			file.write(Data(bytes: &paused, count:MemoryLayout.size(ofValue:paused)))

			file.closeFile()
			}
		catch let error as NSError
			{
			print("Failure to serialise terminal: \(error)")
			}
		}

	/*
		RENDER_TEXT_SCREEN()
		--------------------
	*/
	func render_text_screen()
		{
		screen.render_entire_screen()
		memcpy(img_screen.frame_buffer, screen.bitmap, 480 * 240 * 4)
		img_screen.image = UIImage(cgImage: img_screen.offscreen_bitmap.makeImage()!)
		}

	/*
		DESERIALISE()
		-------------
	*/
	func deserialise(path: URL)
		{
		do
			{
			let file = try FileHandle(forReadingFrom: path)

			try screen.deserialise(file: file)
			try keypad.deserialise(file: file)

			let data = try file.read(upToCount: MemoryLayout.size(ofValue:paused))
			paused = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: paused).self)})

			file.closeFile()
			}
		catch let error as NSError
			{
			print("Failure to deserialise terminal: \(error)")
			}
		}
	}
	
/*
	STRUCT CONTENTVIEW_PREVIEWS
	---------------------------
*/
struct ContentView_Previews: PreviewProvider
	{
	static var previews: some View
		{
		ContentView()
		}
	}
