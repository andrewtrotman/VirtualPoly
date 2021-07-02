/*
	CONTENTVIEW.SWIFT
	-----------------
	Copyright (c) 2021 Andrew Trotman
*/
import SwiftUI
import Foundation
import SpriteKit

let KEYBOARD_POLY = "Poly"
let KEYBOARD_ASCII = "ASCII"

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

class GameScene: SKScene
	{
	let width = 480
	let height = 240
	let background_name = "Background"
	var offscreen_bitmap = [UInt32](repeating: 0, count: 480 * 240)
	var background = SKSpriteNode(imageNamed: "480x240")

	override func didMove(to view: SKView)
		{
		background.name = name
		background.position = CGPoint(x: size.width/2, y: size.height/2)
		background.zPosition = 0

		run(SKAction.repeatForever(SKAction.sequence(
			[
			SKAction.run(
				{
				self.enumerateChildNodes(withName: self.background_name)
					{ (node, _) in
					node.removeFromParent()
					self.addChild(self.background)
					}
				}),
				SKAction.wait(forDuration: 0.5)
			])))
		}

	override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?)
		{
		guard let touch = touches.first else { return }
//		let texture = SKTexture(imageNamed: "background.jpg")


		for x in 0 ..< width
			{
			for y in 0 ..< height
				{
				offscreen_bitmap[y * height + x] = 0xFFFF00FF
				}
			}

		let data = Data(bytes: &offscreen_bitmap, count:width * height * 4)

		let texture = SKTexture(data: data, size: CGSize(width: width, height: height))

		let location = touch.location(in: self)
		let box = SKSpriteNode(texture: texture, size: CGSize(width: 50, height: 50))
		box.position = location
		box.zPosition = 1
		box.physicsBody = SKPhysicsBody(rectangleOf: CGSize(width: 50, height: 50))
		addChild(box)
		}
	}

/*
	STRUCT CONTENTVIEW
	------------------
*/
struct ContentView: View
	{
	var scene: SKScene
		{
		let scene = GameScene()
		scene.size = CGSize(width: 480, height: 240)
		scene.scaleMode = .fill
		return scene
		}

	@Environment(\.scenePhase) var scene_phase
	@State var caps = true				// caps lock is down?
	@State var shift = false			// shift key is down?
	@State var control = false			// cotrol key is down?

	@State var flash_state = false	// Should the cursor be in the visible (or the hidden blink state)?

	let CPU_speed:Int64 = 2000000			// 1,000,000 is 1 MHz
	let iOS_timer_hz:Int64 = 25		// interrupts per second

	@State var flash_timer = Timer.publish(every: 0.5, on: .main, in: .common).autoconnect()
	@State var cpu_timer = Timer.publish(every: 1.0/25.0, on: .main, in: .common).autoconnect()

	@StateObject var machine = machine_changer()
	@State var paused = false

	let poly_img_keyboard = UIImage(named: "PolyKeyboard")!
	let poly_img_keyboard_shift = UIImage(named: "PolyKeyboardShift")!
	let poly_img_keyboard_control = UIImage(named: "PolyKeyboardControl")!
	let poly_img_keyboard_caps = UIImage(named: "PolyKeyboardCaps")!
	let poly_img_keyboard_caps_shift = UIImage(named: "PolyKeyboardCapsShift")!
	let poly_img_keyboard_caps_control = UIImage(named: "PolyKeyboardCapsControl")!

	let ascii_img_keyboard = UIImage(named: "ASCIIKeyboard")!
	let ascii_img_keyboard_shift = UIImage(named: "ASCIIKeyboardShift")!
	let ascii_img_keyboard_control = UIImage(named: "ASCIIKeyboardControl")!
	let ascii_img_keyboard_caps = UIImage(named: "ASCIIKeyboardCaps")!
	let ascii_img_keyboard_caps_shift = UIImage(named: "ASCIIKeyboardCapsShift")!
	let ascii_img_keyboard_caps_control = UIImage(named: "ASCIIKeyboardCapsControl")!

	let keyboard_mode = KEYBOARD_ASCII

	@StateObject var img_screen = image_changer()

	@State var terminal_rendering_width = 40
	@State var terminal_width = 80
	@State var terminal_screen = [UInt8](repeating: 32, count: 80 * 24)
	@State var terminal_row = 0
	@State var terminal_column = 0
	@State var terminal_escape_mode = false
	@State var terminal_escape_sequence = [UInt8](repeating: 32, count: 0)

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
		caps = true
		shift = false
		control = false
		flash_state = false
		terminal_escape_mode = false;
		terminal_row = 0
		terminal_column = 0
		terminal_escape_sequence = []

		terminal_rendering_width = 40

		terminal_screen = [UInt8](repeating: 32, count: 80 * 24)
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
		KEYBOARD_WIDTH()
		----------------
	*/
	func compute_keyboard_width(size: GeometryProxy) -> CGFloat
		{
		let image_size = poly_img_keyboard.size

		let h_ratio = size.size.width / image_size.width
		let v_ratio = size.size.height / image_size.height

		let ratio = h_ratio > v_ratio ? v_ratio : h_ratio

		return image_size.width * ratio
		}

	/*
		KEYBOARD_HEIGHT()
		-----------------
	*/
	func compute_keyboard_height(size: GeometryProxy)  ->	CGFloat
		{
		let image_size = poly_img_keyboard.size

		let h_ratio = size.size.width / image_size.width
		let v_ratio = size.size.height / image_size.height

		let ratio = h_ratio > v_ratio ? v_ratio : h_ratio

		return image_size.height * ratio
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

/*
			SpriteView(scene: scene)
				.frame(width:UIScreen.main.bounds.size.width - frame_size(), height:UIScreen.main.bounds.size.width - frame_size())
            .ignoresSafeArea()
*/
			Image(uiImage: img_screen.image)
				.resizable()
				.frame(width:UIScreen.main.bounds.size.width - frame_size(), height:UIScreen.main.bounds.size.width - frame_size())
				.onAppear(perform:
					{
					img_screen.frame_buffer = img_screen.offscreen_bitmap.data!.assumingMemoryBound(to: UInt8.self)
					render_text_screen()
					img_screen.image = UIImage(cgImage: img_screen.offscreen_bitmap.makeImage()!)
					})

			Spacer().frame(idealHeight: frame_size()).layoutPriority(-1)

			Group
				{
				let keyboard_image_to_use =
					keyboard_mode == KEYBOARD_POLY ?
						caps ?
							shift ? poly_img_keyboard_caps_shift :
							control ? poly_img_keyboard_caps_control :
							poly_img_keyboard_caps :
						shift ? poly_img_keyboard_shift :
						control ?  poly_img_keyboard_control :
						poly_img_keyboard
					:
						caps ?
							shift ? ascii_img_keyboard_caps_shift :
							control ? ascii_img_keyboard_caps_control :
							ascii_img_keyboard_caps :
						shift ? ascii_img_keyboard_shift :
						control ?  ascii_img_keyboard_control :
						ascii_img_keyboard

				GeometryReader
					{ (geometry) in
					VStack
						{
						Divider().frame(minHeight: frame_size(), alignment:.bottom).background(Color.black)
						Image(uiImage: keyboard_image_to_use)
							.resizable()
							.frame(width: compute_keyboard_width(size: geometry), height: compute_keyboard_height(size: geometry), alignment: .bottom)
							.simultaneousGesture(
								DragGesture(minimumDistance: 0, coordinateSpace: .local).onEnded
									{
									let unshift = shift
									let uncontrol = control
									let press = compute_key_press(width: compute_keyboard_width(size: geometry), height: compute_keyboard_height(size: geometry), location: $0.location)
									switch (press)
										{
										case Character("P").asciiValue:		// Pause key
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
											img_screen.image = UIImage(cgImage: img_screen.offscreen_bitmap.makeImage()!)
										case Character("K").asciiValue:		// Control key
											control = !control
											shift = false
										case Character("C").asciiValue:		// Caps lock key
											caps = !caps
										case Character("S").asciiValue:		// Shift key
											shift = true
											control = false
										case Character("F").asciiValue:		// 40 / 80 Column switch
											terminal_rendering_width = terminal_rendering_width == 40 ? 80 : 40
											render_text_screen()
											img_screen.image = UIImage(cgImage: img_screen.offscreen_bitmap.makeImage()!)
										case Character("R").asciiValue:		// Reset button
											reset();
											if (machine.pointer != nil)
												{
												machine_reset(machine.pointer);
												}
											break
										case Character("D").asciiValue:		// ESC key
											if (machine.pointer != nil)
												{
												machine_queue_key_press(machine.pointer, CChar(27))
												}
										case Character("L").asciiValue:		// TAB key
											if (machine.pointer != nil)
												{
												machine_queue_key_press(machine.pointer, CChar(9))
												}
										default:
											if (machine.pointer != nil)
												{
												let ascii = key_translate(key: press, caps_lock: caps, shift: shift, control: control)
												machine_queue_key_press(machine.pointer, CChar(ascii))
												}
										}
									shift = unshift ? false : shift
									control = uncontrol ? false : control
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
									print_character(raw_character: UInt8(response & 0xFF))
									screen_did_change = true
									response = machine_dequeue_serial_output(machine.pointer)
									}

								if screen_did_change
									{
									render_text_screen()
									img_screen.image = UIImage(cgImage: img_screen.offscreen_bitmap.makeImage()!)
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
						img_screen.image = UIImage(cgImage: img_screen.offscreen_bitmap.makeImage()!)
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

			file.write(Data(terminal_screen))

			file.write(Data(bytes: &paused, count:MemoryLayout.size(ofValue:paused)))

			file.write(Data(bytes: &caps, count:MemoryLayout.size(ofValue:caps)))
			file.write(Data(bytes: &shift, count:MemoryLayout.size(ofValue:shift)))
			file.write(Data(bytes: &control, count:MemoryLayout.size(ofValue:control)))

			file.write(Data(bytes: &terminal_row, count:MemoryLayout.size(ofValue:terminal_row)))
			file.write(Data(bytes: &terminal_column, count:MemoryLayout.size(ofValue:terminal_column)))
			file.write(Data(bytes: &terminal_escape_mode, count:MemoryLayout.size(ofValue:terminal_escape_mode)))

			var escape_length = terminal_escape_sequence.count
			file.write(Data(bytes: &escape_length, count:MemoryLayout.size(ofValue:escape_length)))
			file.write(Data(terminal_escape_sequence))

			file.closeFile()
			}
		catch let error as NSError
			{
			print("Failure to serialise terminal: \(error)")
			}
		}

	/*
		DESERIALISE()
		-----------
	*/
	func deserialise(path: URL)
		{
		do
			{
			let file = try FileHandle(forReadingFrom: path)

			var data = try file.read(upToCount: terminal_screen.count)
			data?.copyBytes(to: &terminal_screen, count: terminal_screen.count)

			data = try file.read(upToCount: MemoryLayout.size(ofValue:paused))
			paused = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: paused).self)})

			data = try file.read(upToCount: MemoryLayout.size(ofValue:caps))
			caps = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: caps).self)})

			data = try file.read(upToCount: MemoryLayout.size(ofValue:shift))
			shift = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: shift).self)})

			data = try file.read(upToCount: MemoryLayout.size(ofValue:control))
			control = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: control).self)})

			data = try file.read(upToCount: MemoryLayout.size(ofValue:terminal_row))
			terminal_row = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: terminal_row).self)})

			data = try file.read(upToCount: MemoryLayout.size(ofValue:terminal_column))
			terminal_column = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: terminal_column).self)})

			data = try file.read(upToCount: MemoryLayout.size(ofValue:terminal_escape_mode))
			terminal_escape_mode = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: terminal_escape_mode).self)})


			var escape_length = terminal_escape_sequence.count
			data = try file.read(upToCount: MemoryLayout.size(ofValue:escape_length))
			escape_length = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: escape_length).self)})
			terminal_escape_sequence = [UInt8](repeating: 32, count: escape_length)

			data = try file.read(upToCount: terminal_escape_sequence.count)
			data?.copyBytes(to: &terminal_escape_sequence, count: terminal_escape_sequence.count)

			file.closeFile()
			}
		catch let error as NSError
			{
			print("Failure to deserialise terminal: \(error)")
			}
		}

	/*
		COMPUTE_KEY_PRESS()
		-------------------
		Convert an (x,y) coordinate into key press. Returns the value of the key
		without any shift modifiers, but with:
			C = Caps Lock
			S = Shift
			K = Control

			P = Pause
			E = Enter
			D = ESC
			F = 40/80 column mode
			L = TAB

			A = Char Ins		([ {)
			B = Char Del		(^ ~)
			X = Calc Help		(DC2 GS)
			Y = Exit Back		(\ |)
			Z = Repeat Next	(] })
			G = Left				(US)
			H = Up				(BS)
			I = Down				(VT)
			J = Right			(DC1)
			R = Reset (not on keyboard)
	*/
	func compute_key_press(width: CGFloat, height:CGFloat, location: CGPoint) -> UInt8
		{
		let poly_zero_row =   "ABDXYZLGHIJFR   "
		let poly_first_row =  "1234567890:-P   "
		let poly_second_row = "qwertyuiop^EE   "
		let poly_third_row =  "Casdfghjkl;@K   "
		let poly_fourth_row = "Szxcvbnm,./SS   "

		let ascii_zero_row =   "DLLLAZ\\_ H FR  "
		let ascii_first_row =  "1234567890:-P   "
		let ascii_second_row = "qwertyuiop^EE   "
		let ascii_third_row =  "Casdfghjkl;@K   "
		let ascii_fourth_row = "Szxcvbnm,./SS   "

		let zero_row = keyboard_mode == KEYBOARD_POLY ? poly_zero_row : ascii_zero_row
		let first_row = keyboard_mode == KEYBOARD_POLY ? poly_first_row : ascii_first_row
		let second_row = keyboard_mode == KEYBOARD_POLY ? poly_second_row : ascii_second_row
		let third_row = keyboard_mode == KEYBOARD_POLY ? poly_third_row : ascii_third_row
		let fourth_row = keyboard_mode == KEYBOARD_POLY ? poly_fourth_row : ascii_fourth_row

		let key_width = width / 13.0
		let key_height = height / 6.0

		let row_number = Int(location.y / key_height)
		switch row_number
			{
			case 0:
				let key = Int(location.x / key_width)
				return zero_row[first_row.index(first_row.startIndex, offsetBy: key)].asciiValue!
			case 1:
				let key = Int(location.x / key_width)
				return first_row[first_row.index(first_row.startIndex, offsetBy: key)].asciiValue!
			case 2:
				var key = Int((location.x - key_width / 2) / key_width)
				key = key < 0 ? 0 : key
				return second_row[second_row.index(second_row.startIndex, offsetBy: key)].asciiValue!
			case 3:
				let key = Int(location.x / key_width)
				return third_row[third_row.index(third_row.startIndex, offsetBy: key)].asciiValue!
			case 4:
				var key = Int((location.x - key_width / 2) / key_width)
				key = key < 0 ? 0 : key
				return fourth_row[fourth_row.index(fourth_row.startIndex, offsetBy: key)].asciiValue!
			default:
				return 32		// Space
			}
		}

	/*
		KEY_TRANSLATE()
		---------------
		Turn a key press into the correct ASCII value, managing the <shift> and <caps lock>
		keys appropriately.

		currently unimplemented:
			poly_key_u0 = 0x16;
			poly_key_u1 = 0x15;
			poly_key_u2 = 0x18;
			poly_key_u3 = 0x01;
			poly_key_u4 = 0x7F;
			poly_key_u5 = 0x19;
			poly_key_u6 = 0x06;
			poly_key_u7 = 0x1A;
			poly_key_u8 = 0x0A;
			poly_key_u9 = 0x17;
			poly_key_at = 0x09;
			poly_key_bar = 0x40;
			poly_key_exp = 0x60;
			poly_key_pound = 0x13;
			poly_key_keypad_dot = 0x1E;
	*/
	func key_translate(key: UInt8, caps_lock: Bool, shift: Bool, control: Bool) -> UInt8
		{
		/*
			The "Special" keys
		*/
		switch (key)
			{
			case Character("E").asciiValue:			// Enter
				return 0x0D
			case Character("P").asciiValue:			// Pause
				return shift ? 0x0E  : 0x0F
			case Character("A").asciiValue:			// Insert
				return shift ? 0x7B : 0x5B
			case Character("B").asciiValue:			// Delete
				return shift ? 0x5E : 0x7E
			case Character("X").asciiValue:			// Calc Help
				return shift ? 0x12 : 0x1D
			case Character("Y").asciiValue:			// Exit Back
				return shift ? 0x5C : 0x7C
			case Character("Z").asciiValue:			// Repeat Next
				return shift ? 0x7D : 0x5D
			case Character("G").asciiValue:			// Left
				return 0x1F
			case Character("H").asciiValue:			// Up
				return 0x08
			case Character("I").asciiValue:			// Down
				return 0x0B
			case Character("J").asciiValue:			// Right
				return 0x11
			default:
				break;
			}

		/*
			Shift, Control, and Caps Lock
		*/
		if shift
			{
			/*
				Shift (numbers become symbols)
			*/
			let poly_lower = "1234567890:-^;@,./"
			let poly_upper = "!\"`$%&'()0*=|+#<>?"

			let ascii_lower = "1234567890:-^;@,./\\_"
			let ascii_upper = "!\"#$%&'()0*=|+~<>?\\`"


			let lower = keyboard_mode == KEYBOARD_POLY ? poly_lower : ascii_lower
			let upper = keyboard_mode == KEYBOARD_POLY ? poly_upper : ascii_upper

			if key >= Character("a").asciiValue! && key <= Character("z").asciiValue!
				{
				return key - 32		// convert to uppercase
				}

			let index = lower.distance(from: lower.startIndex, to: lower.firstIndex(of: Character(UnicodeScalar(key)))!)

			return (upper[upper.index(upper.startIndex, offsetBy: index)]).asciiValue!
			}
		else if control
			{
			/*
				Only Control-A to Control-Z (plus a few others) make sense
			*/
			return key >= Character("a").asciiValue! && key <= Character("z").asciiValue! ? key - 96 :
				key == 64 ? 0  :			// NULL
				key == 91 ? 27 :			// Escape
				key == 92 ? 28 :			// File separator
				key == 93 ? 29 :			// Group separator
				key == 94 ? 30 :			// Record separator
				key == 95 ? 31 :			// Unit separator
					key
			}
		else if caps_lock
			{
			/*
				Caps Lock only applies to A through Z
			*/
			return key >= Character("a").asciiValue! && key <= Character("z").asciiValue! ? key - 32 : key
			}

		return key
		}

	/*
		EXTRACT_INTEGER()
		-----------------
	*/
	func extract_integer(sequence: ArraySlice<UInt8>) -> Int
		{
		var result: Int = 0

		for digit in sequence
			{
			result = result * 10 + Int(digit) - 48
			}
//print("->" + String(result))
		return result
		}

	/*
		PRINT_CHARACTER()
		-----------------
		VED uses:
			char bep[1]  = 0x07				/* beep */
			char eol[3]  = ESC [ K			/* Erase to EOL */
			char clr[4]  = ESC [ 2 J		/* Clear screen & pos curser at 1,1 */
			char hom[3]  = ESC [ H			/* Cursor Home */
			char ins[3]  = ESC [ L			/* Insert Line */
			char dell[3] = ESC [ M			/* Delete Line */
	*/
	func print_character(raw_character: UInt8)
		{
		var character = raw_character

		if terminal_escape_mode
			{
			switch raw_character
				{
				case Character("H").asciiValue:			// set cursor position	ESC [ <row> ; <column> H
					if terminal_escape_sequence == []
						{
						terminal_row = 0
						terminal_column = 0
						terminal_escape_sequence.removeAll()
						terminal_escape_mode = false;
						}
					else if terminal_escape_sequence[0] == 91
						{
						var position_of_semicolon = 1
						while terminal_escape_sequence[position_of_semicolon] != 59
							{
							position_of_semicolon = position_of_semicolon + 1
							}
						let new_terminal_row = extract_integer(sequence: terminal_escape_sequence[1 ..< position_of_semicolon])
						let new_terminal_column = extract_integer(sequence: terminal_escape_sequence[(position_of_semicolon + 1)...])
						terminal_row = new_terminal_row <= 24 && new_terminal_row > 0 ? new_terminal_row - 1 : terminal_row
						terminal_column = new_terminal_column <= terminal_rendering_width && new_terminal_column > 0 ? new_terminal_column - 1 : terminal_column
						terminal_escape_sequence.removeAll()
						terminal_escape_mode = false;
						}
//print("GotoYX(" + String(terminal_row) + "," + String(terminal_column) + ")")
					break;
				case Character("J").asciiValue:			// Clear screen
					if (terminal_escape_sequence == [91, 50])		// ESC [ 2 J Erase all of the display
						{
						for x in 0 ..< terminal_width * 24
							{
							terminal_screen[x] = 32
							}
						terminal_escape_sequence.removeAll()
						terminal_escape_mode = false;
						terminal_row = 0
						terminal_column = 0
						}
					break;
				case Character("K").asciiValue:
//print("DeleteEOLN")
					if terminal_escape_sequence == [91]			// ESC [ K erase to end of line (inclusive)
						{
						for pos in terminal_column ..< terminal_width
							{
							terminal_screen[terminal_row * terminal_width + pos] = 32
							}
						terminal_escape_sequence.removeAll()
						terminal_escape_mode = false;
						}
					break;
				case Character("L").asciiValue:					// ESC [ L insert line below
//print("InsertLineBelow")
					if terminal_escape_sequence == []
						{
						if terminal_row != 23
							{
							for y in ((terminal_row + 1) ..< 24).reversed()
								{
								for x in 0 ..< terminal_width
									{
									terminal_screen[y * terminal_width + x] = terminal_screen[(y - 1) * terminal_width + x]
									}
								}
							/*
								Blank the bottom row
							*/
							for byte in (23 * terminal_width) ..< (24 * terminal_width)
								{
								terminal_screen[byte] = 32
								}
							}
						terminal_escape_sequence.removeAll()
						terminal_escape_mode = false;
						}
					break;
				case Character("M").asciiValue:					// ESC [ M delete current line
//print("DeleteLineAt")
					if terminal_escape_sequence == [91]
						{
//						terminal_row = terminal_row == 0 ? 0 : terminal_row - 1
						for x in (terminal_row * terminal_width) ..< (23 * terminal_width)
							{
							terminal_screen[x] = terminal_screen[x + terminal_width]
							}
						/*
							Blank the bottom row
						*/
						for byte in (23 * terminal_width) ..< (24 * terminal_width)
							{
							terminal_screen[byte] = 32
							}
						terminal_escape_sequence.removeAll()
						terminal_escape_mode = false
						}
					break
				default:
					terminal_escape_sequence.append(raw_character)
					terminal_escape_mode = terminal_escape_sequence.count >= 16 ? false : terminal_escape_mode
					break;
				}
			return
			}

//print(String(raw_character))

		if (raw_character == 27)			// ESC (so enter VT100 Escape sequence processing mode)
			{
			terminal_escape_mode = true;
			return
			}

		if raw_character == 0 || raw_character == 7		// 7 is the bell 0 is a null
			{
			return
			}

		if raw_character < 32 || raw_character >= 0x7F
			{
			character = 32;
			}

		/*
			LF (0x0A) is the new line character
		*/
		if raw_character == 0x0A
			{
			terminal_row = terminal_row + 1
			terminal_column = 0;
			}
		/*
			CR (0x0D) is the new line character
		*/
		if raw_character == 0x0D
			{
			terminal_column = 0;
			return;
			}
		/*
			BS (0x08) is the backspace character
		*/
		if raw_character == 0x08
			{
			terminal_column = terminal_column > 0 ? terminal_column - 1 : 0
			terminal_screen[terminal_row * terminal_width + terminal_column] = character
			return;
			}
		/*
			Check for scrolling
		*/
		if (terminal_column >= terminal_rendering_width)
			{
			terminal_column = 0
			terminal_row = terminal_row + 1
			}
		if (terminal_row >= 24)
			{
			/*
				Scroll
			*/
			for byte in terminal_width ..< (terminal_width * 24)
				{
				terminal_screen[byte - terminal_width] = terminal_screen[byte]
				}
			/*
				Blank the bottom row
			*/
			for byte in (23 * terminal_width) ..< (24 * terminal_width)
				{
				terminal_screen[byte] = 32
				}
			/*
				Go to the bottom row
			*/
			terminal_row = 23
			}

		if raw_character != 0x0D && raw_character != 0x0A && raw_character != 0x7F
			{
			terminal_screen[terminal_row * terminal_width + terminal_column] = character
			terminal_column = terminal_column + 1
			}
		}

	/*
		RENDER_TEXT_SCREEN()
		--------------------
	*/
	func render_text_screen()
		{
		if (terminal_rendering_width == 40)
			{
			for y in 0 ..< 24
				{
				for x in 0 ..< terminal_rendering_width
					{
					draw_screen_40(screen_x: x, screen_y: y, character: terminal_screen[y * terminal_width + x])
					}
				}
			}
		else
			{
			for y in 0 ..< 24
				{
				for x in 0 ..< terminal_rendering_width
					{
					draw_screen_80(screen_x: x, screen_y: y, character: terminal_screen[y * terminal_width + x])
					}
				}
			}
		}

	/*
		DRAW_SCREEN_40()
		----------------
		40-column text screen renderer

		red = dataType[offset]
		green   = dataType[offset + 1]
		blue = dataType[offset + 2]
		alpha  = dataType[offset + 3]
	*/
	func draw_screen_40(screen_x: Int, screen_y: Int, character: UInt8)
		{
		let pixel_map = img_screen.offscreen_bitmap.data!.assumingMemoryBound(to: UInt32.self)
		var on: UInt32 = 0x00FFFFFF
		var off: UInt32 = 0x00000000
		let glyph_base = 0
		let from = (Int(character) - 32 + glyph_base) * 10

		if flash_state && screen_y == terminal_row
			{
			if screen_x == terminal_column || (screen_x == 39 && terminal_column >= 39)
				{
				swap(&on, &off);
				}
			}

		for y in 0 ..< 10
			{
			let pos = get_saa5050_font()[from + y]
			var into = (screen_x * 12 + screen_y * 10 * 480) + (y * 480)

			for x in 2 ..< 8
				{
				if (pos & (0x80 >> x)) != 0
					{
					pixel_map[into] = on
					pixel_map[into + 1] = on
					into = into + 2
					}
				else
					{
					pixel_map[into] = off
					pixel_map[into + 1] = off
					into = into + 2
					}
				}
			}
		}

	/*
		DRAW_SCREEN_80()
		----------------
		80-column text screen renderer
		red = dataType[offset]
		green   = dataType[offset + 1]
		blue = dataType[offset + 2]
		alpha  = dataType[offset + 3]
	*/
	func draw_screen_80(screen_x: Int, screen_y: Int, character: UInt8)
		{
		let pixel_map = img_screen.offscreen_bitmap.data!.assumingMemoryBound(to: UInt32.self)
		var on: UInt32 = 0x00FFFFFF
		var off: UInt32 = 0x00000000
		let glyph_base = 0
		let from = (Int(character) - 32 + glyph_base) * 10

		if flash_state && screen_y == terminal_row
			{
			if screen_x == terminal_column || (screen_x == terminal_rendering_width - 1 && terminal_column >= terminal_rendering_width - 1)
				{
				swap(&on, &off);
				}
			}

		for y in 0 ..< 10
			{
			let pos = get_saa5050_font()[from + y]
			var into = (screen_x * 6 + screen_y * 10 * 480) + (y * 480)

			for x in 2 ..< 8
				{
				if (pos & (0x80 >> x)) != 0
					{
					pixel_map[into] = on
					into = into + 1
					}
				else
					{
					pixel_map[into] = off
					into = into + 1
					}
				}
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
