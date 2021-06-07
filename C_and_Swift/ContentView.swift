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
	let c_ob = something_construct()
	let img = UIImage(named: "PolyKeyboard")!
	let img_caps = UIImage(named: "PolyKeyboardShift")!
	var body: some View
		{
		VStack
			{
			Text(textToUpdate).padding()
			Group{
			Image(uiImage: (caps || shift) ? img_caps : img).resizable().aspectRatio(contentMode: .fit).simultaneousGesture(
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
						default:
							let ascii = key_upper(key: press, caps_lock: caps, shift: shift)
							self.textToUpdate = self.textToUpdate + String(ascii)
						}
					if unshift
						{
						shift = false
						}
					}
				)
			}.frame(maxHeight: .infinity, alignment: .bottom)
			Spacer().frame(maxHeight: 2)
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
