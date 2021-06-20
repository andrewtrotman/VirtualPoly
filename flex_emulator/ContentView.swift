/*
	CONTENTVIEW.SWIFT
	-----------------
	Copyright (c) 2021 Andrew Trotman
*/
import SwiftUI

class image_changer: ObservableObject
	{
	var counter = 0

	let offscreen_bitmap = CGContext(data: malloc(480*240*4), width: 480, height: 240, bitsPerComponent: 8, bytesPerRow: 480 * 4, space: CGColorSpaceCreateDeviceRGB(), bitmapInfo: CGImageAlphaInfo.noneSkipLast.rawValue)!
	@Published public var image = UIImage(named: "480x240")!
	public var frame_buffer: UnsafeMutablePointer<UInt8>?

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
	@State var caps = true				// caps lock is down?
	@State var shift = false			// shift key is down?
	@State var control = false			// cotrol key is down?

	@State var flash_state = false	// Should the cursor be in the visible (or the hidden blink state)?

	let CPU_speed:Int64 = 3000000			// 1000000 is 1 MHz
	let iOS_timer_speed:Int64 = 100		// do CPU_speed/iOS_timer_speed cycles per timer interrupt
	let timer = Timer.publish(every: 0.01, on: .main, in: .common).autoconnect()
	let flash_timer = Timer.publish(every: 0.5, on: .main, in: .common).autoconnect()

	let machine = machine_construct()

	let img_keyboard = UIImage(named: "PolyKeyboard")!
	let img_keyboard_shift = UIImage(named: "PolyKeyboardShift")!
	let img_keyboard_control = UIImage(named: "PolyKeyboardControl")!
	let img_keyboard_caps = UIImage(named: "PolyKeyboardCaps")!
	let img_keyboard_caps_shift = UIImage(named: "PolyKeyboardCapsShift")!
	let img_keyboard_caps_control = UIImage(named: "PolyKeyboardCapsControl")!

	@StateObject var img_screen = image_changer()

	@State var terminal_screen = [UInt8](repeating: 33, count: 40 * 24)
	@State var terminal_row = 0
	@State var terminal_column = 0

	var body: some View
		{
		VStack
			{
			Image(uiImage: img_screen.image).resizable().frame(width:UIScreen.main.bounds.size.width - 5, height:UIScreen.main.bounds.size.width - 5).onAppear(perform:
				{
				for x in 0 ..< (40 * 24)
					{
					terminal_screen[x] = 32		// fill with spaces at the start
					}
				img_screen.frame_buffer = img_screen.offscreen_bitmap.data!.assumingMemoryBound(to: UInt8.self)
				render_text_screen()
				img_screen.image = UIImage(cgImage: img_screen.offscreen_bitmap.makeImage()!)
				})
			Group{
			let keyboard_image_to_use =
				caps ?
					shift ? img_keyboard_caps_shift :
					control ? img_keyboard_caps_control :
					img_keyboard_caps :
				shift ? img_keyboard_shift :
				control ?  img_keyboard_control :
				img_keyboard

			Image(uiImage: keyboard_image_to_use).resizable().aspectRatio(contentMode: .fit).simultaneousGesture(
				DragGesture(minimumDistance: 0, coordinateSpace: .local).onEnded
					{
					let unshift = shift
					let uncontrol = control
					let factor = img_keyboard.size.width / UIScreen.main.bounds.width
					let geometry: CGSize = CGSize(width: img_keyboard.size.width / factor, height: img_keyboard.size.height / factor)
					let press = compute_key_press(size: geometry, location: $0.location)
					switch (press)
						{
						case Character("K").asciiValue:
							control = !control
							shift = false
						case Character("C").asciiValue:
							caps = !caps
						case Character("S").asciiValue:
							shift = true
							control = false
						default:
							let ascii = key_translate(key: press, caps_lock: caps, shift: shift, control: control)
							machine_queue_key_press(machine, CChar(ascii))
						}
					shift = unshift ? false : shift
					control = uncontrol ? false : control
					}
				)
			}.frame(maxHeight: .infinity, alignment: .bottom)
			
			Spacer().frame(maxHeight: 2).onReceive(timer)
				{ _ in
				let end_cycle = machine_cycles_spent(machine) + CPU_speed / iOS_timer_speed
				while (machine_cycles_spent(machine) < end_cycle)
					{
					machine_step(machine);
					}

				var screen_did_change = false
				var response = machine_dequeue_serial_output(machine)
				while (response <= 0xFF)
					{
					print_character(raw_character: UInt8(response & 0xFF))
					screen_did_change = true
					response = machine_dequeue_serial_output(machine)
					}

				if screen_did_change
					{
					render_text_screen()
					img_screen.image = UIImage(cgImage: img_screen.offscreen_bitmap.makeImage()!)
					}
				}
			.onReceive(flash_timer)
				{ _ in
				flash_state = !flash_state
				render_text_screen()
				img_screen.image = UIImage(cgImage: img_screen.offscreen_bitmap.makeImage()!)
				}
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

			A = Char Ins
			B = Char Del
			X = Calc Help
			Y = Exit Back
			Z = Repeat Next
			G = Left
			H = Up
			I = Down
			J = Right
			R = Reset (not on keyboard)
	*/
	func compute_key_press(size: CGSize, location: CGPoint) -> UInt8
		{
		let zero_row =   "AB XYZ GHIJ R   "
		let first_row =  "1234567890:-P   "
		let second_row = "qwertyuiop^EE   "
		let third_row =  "Casdfghjkl;@K   "
		let fourth_row = "Szxcvbnm,./SS   "
		let key_width = size.width / 13.0
		let key_height = size.height / 6.0

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
			let lower = "1234567890:-^;@,./"
			let upper = "!\"`$%&'()0*=|+#<>?"

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
				Only Control-A to Control-Z make sense
			*/
			return key >= Character("a").asciiValue! && key <= Character("z").asciiValue! ? key - 96 : key
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
		PRINT_CHARACTER()
		-----------------
	*/
	func print_character(raw_character: UInt8)
		{
		var character = raw_character

		if raw_character == 0
			{
			return
			}
		if raw_character < 32 || raw_character >= 0x7F
			{
			character = 32;
			}

		/*
			CR (0x0D) is the new line character
		*/
		if raw_character == 0x0A
			{
			terminal_row = terminal_row + 1
			terminal_column = 0;
			}
		/*
			BS (0x08) is the backspace character
		*/
//		if raw_character == 0x08
		if raw_character == 0x1F
			{
			terminal_column = terminal_column > 0 ? terminal_column - 1 : 0
			terminal_screen[terminal_row * 40 + terminal_column] = character
			return;
			}
		/*
			Check for scrolling
		*/
		if (terminal_column >= 40)
			{
			terminal_column = 0
			terminal_row = terminal_row + 1
			}
		if (terminal_row >= 24)
			{
			/*
				Scroll
			*/
			for byte in 40 ..< (40 * 24)
				{
				terminal_screen[byte - 40] = terminal_screen[byte]
				}
			/*
				Blank the bottom row
			*/
			for byte in (23 * 40) ..< (24 * 40)
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
			terminal_screen[terminal_row * 40 + terminal_column] = character
			terminal_column = terminal_column + 1
			}
		}

	/*
		RENDER_TEXT_SCREEN()
		--------------------
	*/
	func render_text_screen()
		{
		for x in 0 ..< 40
			{
			for y in 0 ..< 24
				{
				drawContentIntoBitmap(screen_x: x, screen_y: y, character: terminal_screen[y * 40 + x])
				}
			}
		}

	/*
		DRAWCONTENTINTOBITMAP()
		-----------------------
		red = dataType[offset]
		green   = dataType[offset + 1]
		blue = dataType[offset + 2]
		alpha  = dataType[offset + 3]
	*/
	func drawContentIntoBitmap(screen_x: Int, screen_y: Int, character: UInt8)
		{
		let pixel_map = img_screen.offscreen_bitmap.data!.assumingMemoryBound(to: UInt32.self)
		var on: UInt32 = 0x00FFFFFF
		var off: UInt32 = 0x00000000
		let glyph_base = 0
		let from = (Int(character) - 32 + glyph_base) * 10

		if (screen_x == terminal_column && screen_y == terminal_row && flash_state)
			{
			swap(&on, &off);
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
