/*
	CONTENTVIEW.SWIFT
	-----------------
	Copyright (c) 2021 Andrew Trotman
*/
import SwiftUI

/*
	STRUCT CONTENTVIEW
	------------------
*/
struct ContentView: View
	{
	@State var caps = false				// caps lock is down
	@State var shift = false			// shift key is down
	@State var textToUpdate = ""

	@State var frame_buffer: UnsafeMutablePointer<UInt8>?
	let offscreen_bitmap = CGContext(data: malloc(480*240*4), width: 480, height: 240, bitsPerComponent: 8, bytesPerRow: 480 * 4, space: CGColorSpaceCreateDeviceRGB(), bitmapInfo: CGImageAlphaInfo.noneSkipLast.rawValue)!
	let timer = Timer.publish(every: 0.0, on: .main, in: .common).autoconnect()

	let machine = machine_construct()
	let img = UIImage(named: "PolyKeyboard")!
	let img_caps = UIImage(named: "PolyKeyboardShift")!
	@State var img_screen = UIImage(named: "480x240")!

	@State var terminal_screen = [UInt8](repeating: 33, count: 40 * 24)
	@State var terminal_row = 0
	@State var terminal_column = 0

	var body: some View
		{
		VStack
			{
//			Text(textToUpdate).padding().font(.system(size: 10, weight: .heavy, design: .monospaced))
			Image(uiImage: img_screen).resizable().frame(width:UIScreen.main.bounds.size.width - 5, height:UIScreen.main.bounds.size.width - 5).onAppear(perform:
				{
				for x in 0 ..< (40 * 24)
					{
					terminal_screen[x] = UInt8((x % 26) + 65)
					}
				frame_buffer = offscreen_bitmap.data!.assumingMemoryBound(to: UInt8.self)
				render_text_screen()
				img_screen = UIImage(cgImage: offscreen_bitmap.makeImage()!)
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
						case "C":
							caps = !caps
						case "S":
							shift = true
						case "P":
							self.textToUpdate = "<PAUSE>"
						case "E":
							self.textToUpdate = ""
							let ascii = ("\n" as Character).asciiValue!
							machine_queue_key_press(machine, Int8(ascii))
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
				for _ in 1...100
					{
					machine_step(machine);
					}

				var response: Int32 = 0
				repeat
					{
					response = machine_dequeue_serial_output(machine)
					if (response <= 0xFF)
						{
						print_character(raw_character: UInt8(response & 0xFF))
						self.textToUpdate = self.textToUpdate + String(UnicodeScalar(UInt8(response)))
						render_text_screen()
						img_screen = UIImage(cgImage: offscreen_bitmap.makeImage()!)
						}
					}
				while response <= 0xFF
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
	*/
	func compute_key_press(size: CGSize, location: CGPoint) -> Character
		{
		let first_row =  "1234567890:-P   "
		let second_row = "qwertyuiop^EE   "
		let third_row =  "Casdfghjkl;@@   "
		let fourth_row = "Szxcvbnm,./SS   "
		let key_width = size.width / 13.0
		let key_height = size.height / 5.0

		let row_number = Int(location.y / key_height)
		switch row_number
			{
			case 0:
				let key = Int(location.x / key_width)
				return first_row[first_row.index(first_row.startIndex, offsetBy: key)]
			case 1:
				var key = Int((location.x - key_width / 2) / key_width)
				key = key < 0 ? 0 : key
				return second_row[second_row.index(second_row.startIndex, offsetBy: key)]
			case 2:
				let key = Int(location.x / key_width)
				return third_row[third_row.index(third_row.startIndex, offsetBy: key)]
			case 3:
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
		let character = (raw_character < 32 || raw_character > 0x7F) ? 32 : raw_character

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
		terminal_screen[terminal_row * 40 + terminal_column] = character
		terminal_column = terminal_column + 1
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
		let pixel_map = offscreen_bitmap.data!.assumingMemoryBound(to: UInt32.self)
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
