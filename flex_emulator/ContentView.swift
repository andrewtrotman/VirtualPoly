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
	@State var caps = false				// caps lock is down
	@State var shift = false			// shift key is down
	@State var control = false			// cotrol key is down

	let CPU_speed:Int64 = 3000000			// 1000000 is 1 MHz
	let iOS_timer_speed:Int64 = 100		// do CPU_speed/iOS_timer_speed cycles per timer interrupt
	let timer = Timer.publish(every: 0.01, on: .main, in: .common).autoconnect()

	let machine = machine_construct()
	let img = UIImage(named: "PolyKeyboard")!
	let img_caps = UIImage(named: "PolyKeyboardShift")!
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
			let keyboard_image_to_use = (caps || shift) ? img_caps : img
			Image(uiImage: keyboard_image_to_use).resizable().aspectRatio(contentMode: .fit).simultaneousGesture(
				DragGesture(minimumDistance: 0, coordinateSpace: .local).onEnded
					{
					let unshift = shift
					let factor = img.size.width / UIScreen.main.bounds.width
					let geometry: CGSize = CGSize(width: img.size.width / factor, height: img.size.height / factor)
					let press = compute_key_press(size: geometry, location: $0.location)
					switch (press)
						{
						case "K":
							control = !control
						case "C":
							caps = !caps
						case "S":
							shift = true
						case "P":
							break
						case "E":
							machine_queue_key_press(machine, 0x0D)
						default:
							let ascii = key_upper(key: press, caps_lock: caps, shift: shift)
							machine_queue_key_press(machine, Int8(ascii.asciiValue!))
						}
					if unshift
						{
						shift = false
						}
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
			}
		}

	/*
		COMPUTE_KEY_PRESS()
		-------------------
		Convert an (x,y) coordinate into key press. Returns the value of the key
		without any shift modifiers, but with:
			P = Pause
			E = Enter
			C = Caps Lock
			S = Shift
			K = Control

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

	poly_key_line_insert = 0x7B;
	poly_key_char_insert = 0x5B;
	poly_key_line_del = 0x5E;
	poly_key_char_del = 0x7E;
	poly_key_calc = 0x12;
	poly_key_help = 0x1D;
	poly_key_exit = 0x5C;
	poly_key_back = 0x7C;
	poly_key_repeat = 0x7D;
	poly_key_next = 0x5D;
	poly_key_pause = 0x0F;
	poly_key_left = 0x1F;
	poly_key_right = 0x11;
	poly_key_up = 0x08;
	poly_key_down = 0x0B;
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

	// Poly 2 keyboard only
	poly_key_shift_pause = 0x0E;
	poly_key_keypad_dot = 0x1E;
	*/
	func compute_key_press(size: CGSize, location: CGPoint) -> Character
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
				return zero_row[first_row.index(first_row.startIndex, offsetBy: key)]
			case 1:
				let key = Int(location.x / key_width)
				return first_row[first_row.index(first_row.startIndex, offsetBy: key)]
			case 2:
				var key = Int((location.x - key_width / 2) / key_width)
				key = key < 0 ? 0 : key
				return second_row[second_row.index(second_row.startIndex, offsetBy: key)]
			case 3:
				let key = Int(location.x / key_width)
				return third_row[third_row.index(third_row.startIndex, offsetBy: key)]
			case 4:
				var key = Int((location.x - key_width / 2) / key_width)
				key = key < 0 ? 0 : key
				return fourth_row[fourth_row.index(fourth_row.startIndex, offsetBy: key)]
			default:
				return " "
			}
		}

	/*
		KEY_UPPER()
		-----------
		Turn a key press into the correct ASCII value, managing the <shift> and <caps lock>
		keys appropriately.
	*/
	func key_upper(key: Character, caps_lock: Bool, shift: Bool) -> Character
		{
		if shift
			{
			let lower = "1234567890:-^;@,./"
			let upper = "!\"`$%&'()0*=|+#<>?"

			if key >= "a" && key <= "z"
				{
				return Character(key.uppercased())
				}

			let index = lower.distance(from: lower.startIndex, to: lower.firstIndex(of: key)!)
			return upper[upper.index(upper.startIndex, offsetBy: index)]
			}
		else if caps_lock
			{
			return key >= "a" && key <= "z" ? Character(key.uppercased()) : key
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

		if (raw_character == 0)
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
		let on: UInt32 = 0x00FFFFFF
		let off: UInt32 = 0x00000000
		let glyph_base = 0
		let from = (Int(character) - 32 + glyph_base) * 10

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
