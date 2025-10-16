/*
	CONTENTVIEW.SWIFT
	-----------------
	Copyright (c) 2021 Andrew Trotman
*/
import SwiftUI
import Foundation

/*
	CLASS IMAGE_CHANGER
	-------------------
*/
class image_changer: ObservableObject
	{
	let offscreen_bitmap = CGContext(data: malloc(480 * 240 * 4), width: 480, height: 240, bitsPerComponent: 8, bytesPerRow: 480 * 4, space: CGColorSpaceCreateDeviceRGB(), bitmapInfo: CGImageAlphaInfo.noneSkipLast.rawValue)!
	@Published public var image = UIImage(named: "480x240")!
	public var frame_buffer: UnsafeMutablePointer<UInt8>?

	/*
		INIT()
		------
	*/
	init()
		{
		frame_buffer = offscreen_bitmap.data!.assumingMemoryBound(to: UInt8.self)
		}
	}

/*
	CLASS MACHINE_CHANGER
	---------------------
*/
class machine_changer: ObservableObject
	{
	@Published public var pointer: UnsafeRawPointer? = nil

	/*
		INIT()
		------
	*/
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

	let CPU_speed: UInt64 = 20000000			// 1,000,000 is 1 MHz
//	let CPU_speed: UInt64 = 1000000			// 1,000,000 is 1 MHz
	let iOS_timer_hz: UInt64 = 25		// interrupts per second
    
	@State var flash_timer = Timer.publish(every: 0.5, on: .main, in: .common).autoconnect()
	@State var cpu_timer = Timer.publish(every: 1.0 / 25.0, on: .main, in: .common).autoconnect()
	@State var one_second_timer = Timer.publish(every: 1.0, on: .main, in: .common).autoconnect()

	@State var machine = machine_changer()
	@State var paused = false								// the 6809 is paused
	@State var inactive = false							// iOS has made us inactive or background

	@State var screen: screen_base? = nil

	@State var keypad = keyboard()

	@StateObject var img_screen = image_changer()

	@State var initial_time = NSDate()
	@State var previous_time = NSDate()
	@State var previous_cycle_count: UInt64 = 0

	/*
		INIT()
		------
	*/
	init()
		{
		stop_timers()
		initial_time = NSDate()
		previous_time = NSDate()
		reset()
		inactive = false
		start_timers()
		}

	/*
		RESET()
		-------
	*/
	func reset()
		{
		previous_cycle_count = 0
		paused = false;
		if machine.pointer != nil
			{
			machine_reset(machine.pointer);
			}
		if (screen != nil)
			{
			screen!.reset()
			}
		keypad.reset()
		}

	/*
		START_TIMERS()
		--------------
	*/
	func start_timers()
		{
		one_second_timer = Timer.publish(every: 1.0, on: .main, in: .common).autoconnect()
		flash_timer = Timer.publish(every: 0.5, on: .main, in: .common).autoconnect()
		cpu_timer = Timer.publish(every: 1.0 / Double(iOS_timer_hz), on: .main, in: .common).autoconnect()
		}

	/*
		STOP_TIMERS()
		-------------
	*/
	func stop_timers()
		{
		cpu_timer.upstream.connect().cancel()
		flash_timer.upstream.connect().cancel()
		one_second_timer.upstream.connect().cancel()
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
			Spacer()
				.frame(maxHeight: frame_size())
				.layoutPriority(1)

			Image(uiImage: img_screen.image)
				.resizable()
				.frame(width:UIScreen.main.bounds.size.width - frame_size(), height:UIScreen.main.bounds.size.width - frame_size())
				.onAppear(perform:
					{
					render_text_screen()
					})

			Spacer()
				.frame(idealHeight: frame_size())
				.layoutPriority(-1)

			Group
				{
				let keyboard_image_to_use = keypad.keyboard_image()

				GeometryReader
					{ (geometry) in
					VStack
						{
						Divider()
							.frame(minHeight: frame_size(), alignment:.bottom)
							.background(Color.black)

						Image(uiImage: keyboard_image_to_use)
							.resizable()
							.frame(width: keypad.width(size: geometry.size), height: keypad.height(size: geometry.size), alignment: .bottom)
							.simultaneousGesture(
								DragGesture(minimumDistance: 0, coordinateSpace: .local)
								.onEnded
									{
									let ascii = keypad.keyboard_press(width: keypad.width(size: geometry.size), height: keypad.height(size: geometry.size), location: $0.location)
									switch (ascii)
										{
										case keyboard.key.KEY_NONE.rawValue:
											break
										case keyboard.key.KEY_PAUSE.rawValue:
											if (!paused)
												{
												stop_timers()
												}
											else
												{
												start_timers()
												machine_rewind_cycles_spent(machine.pointer)
												initial_time = NSDate()
												previous_time = NSDate()
												previous_cycle_count = machine_cycles_spent(machine.pointer)
												}
											paused = !paused
											/*
												Turn the cursor on and render the screen state as it is.
											*/
											screen!.flash_state = true
											render_text_screen()
										case keyboard.key.KEY_40_80.rawValue:		// 40 / 80 Column switch
											screen!.set_width(new_width: screen!.get_width() == .eighty ? .fourty : .eighty)
											render_text_screen()
										case keyboard.key.KEY_RESET.rawValue:		// Reset button
											reset();
										default:
											machine_queue_key_press(machine.pointer, CChar(ascii))
											machine_queue_key_release(machine.pointer, CChar(ascii))
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
					if (!paused && !inactive)
						{
						var screen_did_change = false

						/*
							 The old method of keeping track of the clock cycles involved mapping clock time
							 to cycles so that the emulator could catch up or slow down based on how busy the
							 device is.  This isn't so good in the debugger because time doesn't stop and so
							 it hangs when returning.

							 An alternative method is to just add a time-slice worth of cycles and run to thee
							 but this results in clock drift.

							 The chosen solution is the first approach, but to jump the cycle count if we detect
							 that something has gone wrong (i.e. we stopped for unexpected reasond.
						*/
                  // First approach
//						let total_seconds_count = -initial_time.timeIntervalSinceNow
//						let end_cycle = UInt64(Double(CPU_speed) * total_seconds_count)
                        
						// Second approach
//                      let end_cycle = machine_cycles_spent(machine.pointer) + CPU_speed / iOS_timer_hz

						// Third approach
						let total_seconds_count = -initial_time.timeIntervalSinceNow
						let end_cycle = UInt64(Double(CPU_speed) * total_seconds_count)

						if end_cycle > machine_cycles_spent(machine.pointer) + 10 * (CPU_speed / iOS_timer_hz)
							{
							machine_set_cycles_spent(machine.pointer, UInt64(Double(CPU_speed) * total_seconds_count) - (CPU_speed / iOS_timer_hz))
							}

						while (machine_cycles_spent(machine.pointer) < end_cycle)
							{
							machine_step(machine.pointer, CPU_speed / iOS_timer_hz / 8);

							var response = machine_dequeue_serial_output(machine.pointer)
							while (response <= 0xFF)
								{
								screen!.print_character(raw_character: UInt8(response & 0xFF))
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
				.onReceive(flash_timer)
					{ _ in
					if !paused
						{
						screen!.flash_state.toggle()
						render_text_screen()
						}
					}
				.onReceive(one_second_timer)
					{ _ in
//					let total_cycles_spent: UInt64 = UInt64(machine_cycles_spent(machine.pointer))
//					let slice_cycles_spent: UInt64 = total_cycles_spent - previous_cycle_count
//
//					let total_seconds_count = -initial_time.timeIntervalSinceNow
//					let current_seconds_count = -previous_time.timeIntervalSinceNow
//
//					print("total=\((Double(total_cycles_spent) / total_seconds_count / 1000.0).rounded()) KHz now=\((Double(slice_cycles_spent) / current_seconds_count / 1000.0).rounded()) KHz \n");

//					previous_time = NSDate()
//					previous_cycle_count = machine_cycles_spent(machine.pointer)
					}
				.onAppear
					{
					if (machine.pointer == nil)
						{
//						machine.pointer = machine_construct(ARROW, 1, 1)
//						screen = screen_arrow()

//						machine.pointer = machine_construct(PINNATED, 1, 1)
//						screen = terminal()

						/*
							Poly ROM versions are: 23, 30, 31, 34, 341.  341 is 34 with local disk drive and RED login screen
						*/
//						machine.pointer = machine_construct(POLY, 34, 1)		// ROM version 3.4

						machine.pointer = machine_construct(POLY_WITH_PROTEUS, 34, 30982)		// ROM version 3.4
						let poly_screen = screen_poly()
						poly_screen.set_machine(poly: machine.pointer)
						screen = poly_screen

						screen!.set_screen_buffer(screen_buffer: machine_get_screen_buffer(machine.pointer))
//						machine_deserialise(machine.pointer)
//						deserialise(path: get_serialised_filename())
						reset()
						}
					}
				.onChange(of: scene_phase)
					{ new_phase in
					switch new_phase
						{
						case .active:
							inactive = false
							machine_deserialise(machine.pointer)
							deserialise(path: get_serialised_filename())
						case .inactive, .background:
							inactive = true
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
		RENDER_TEXT_SCREEN()
		--------------------
	*/
	func render_text_screen()
		{
		if (screen != nil)
			{
			screen!.render_entire_screen()
			memcpy(img_screen.frame_buffer, screen!.bitmap, 480 * 240 * 4)
			img_screen.image = UIImage(cgImage: img_screen.offscreen_bitmap.makeImage()!)
			}
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

			screen!.serialise(file: file)
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
		DESERIALISE()
		-------------
	*/
	func deserialise(path: URL)
		{
		do
			{
			let file = try FileHandle(forReadingFrom: path)

			try screen!.deserialise(file: file)
			try keypad.deserialise(file: file)

			let data = try file.read(upToCount: MemoryLayout.size(ofValue:paused))
			paused = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: paused).self)})

			file.closeFile()

			initial_time = NSDate()
			previous_time = NSDate()
			previous_cycle_count = machine_cycles_spent(machine.pointer)
			}
		catch let error as NSError
			{
			print("Failure to deserialise terminal: \(error)")
			}
		}
	}
