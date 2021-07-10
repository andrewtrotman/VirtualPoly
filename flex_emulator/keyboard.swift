/*
	KEYBOARD.SWIFT
	--------------
	Copyright (c) 2021 Andrew Trotman
*/
import UIKit
import Foundation

let KEYBOARD_POLY = "Poly"
let KEYBOARD_ASCII = "ASCII"
let keyboard_mode = KEYBOARD_ASCII

/*
	CLASS KEYBOARD
	--------------
*/
class keyboard
	{
	enum key: UInt8
		{
		case KEY_NONE   = 0x00
		case KEY_TAB    = 0x09
		case KEY_ESCAPE = 0x1B
		case KEY_RESET  = 0xFD
		case KEY_40_80  = 0xFE
		case KEY_PAUSE  = 0xFF
		}
		
	private let image_keyboard = UIImage(named: keyboard_mode + "Keyboard")!
	private let image_keyboard_shift = UIImage(named: keyboard_mode + "KeyboardShift")!
	private let image_keyboard_control = UIImage(named: keyboard_mode + "KeyboardControl")!
	private let image_keyboard_caps = UIImage(named: keyboard_mode + "KeyboardCaps")!
	private let image_keyboard_caps_shift = UIImage(named: keyboard_mode + "KeyboardCapsShift")!
	private let image_keyboard_caps_control = UIImage(named: keyboard_mode + "KeyboardCapsControl")!

	private var caps = true				// caps lock is down?
	private var shift = false				// shift key is down?
	private var control = false			// cotrol key is down?

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
		caps = true
		shift = false
		control = false
		}

	/*
		IMAGE_SCALING_RATIO()
		---------------------
	*/
	private func image_scaling_ratio(size: CGSize) -> CGFloat
		{
		let image_size = image_keyboard.size

		let h_ratio = size.width / image_size.width
		let v_ratio = size.height / image_size.height

		return h_ratio > v_ratio ? v_ratio : h_ratio
		}

	/*
		WIDTH()
		-------
	*/
	func width(size: CGSize) -> CGFloat
		{
		let ratio = image_scaling_ratio(size: size)
		return image_keyboard.size.width * ratio
		}

	/*
		HEIGHT()
		--------
	*/
	func height(size: CGSize)  ->	CGFloat
		{
		let ratio = image_scaling_ratio(size: size)
		return image_keyboard.size.height * ratio
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
	private func compute_key_press(width: CGFloat, height:CGFloat, location: CGPoint) -> UInt8
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
		COMPUTE_KEY_TRANSLATION()
		-------------------------
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
	private func compute_key_translation(key: UInt8, caps_lock: Bool, shift: Bool, control: Bool) -> UInt8
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
		KEYBOARD_PRESS()
		----------------
	*/
	func keyboard_press(width: CGFloat, height:CGFloat, location: CGPoint) -> UInt8
		{
		var ascii: UInt8 = 0
		let unshift = shift
		let uncontrol = control
		let press = compute_key_press(width: width, height: height, location: location)

		switch (press)
			{
			case Character("P").asciiValue:		// Pause key
				ascii = key.KEY_PAUSE.rawValue
			case Character("K").asciiValue:		// Control key
				control = !control
				shift = false
				ascii = key.KEY_NONE.rawValue
			case Character("C").asciiValue:		// Caps lock key
				caps = !caps
				ascii = key.KEY_NONE.rawValue
			case Character("S").asciiValue:		// Shift key
				shift = true
				control = false
				ascii = key.KEY_NONE.rawValue
			case Character("F").asciiValue:		// 40 / 80 Column switch
				ascii = key.KEY_40_80.rawValue
			case Character("R").asciiValue:		// Reset button
				ascii = key.KEY_RESET.rawValue
			case Character("D").asciiValue:		// ESC key
				ascii = key.KEY_ESCAPE.rawValue
			case Character("L").asciiValue:		// TAB key
				ascii = key.KEY_TAB.rawValue
			default:
				ascii = compute_key_translation(key: press, caps_lock: caps, shift: shift, control: control)
			}

		shift = unshift ? false : shift
		control = uncontrol ? false : control

		return ascii
		}

	/*
		KEYBOARD_IMAGE()
		----------------
	*/
	func keyboard_image() -> UIImage
		{
		return caps ?
					shift ? image_keyboard_caps_shift :
					control ? image_keyboard_caps_control :
					image_keyboard_caps :
				shift ? image_keyboard_shift :
				control ?  image_keyboard_control :
				image_keyboard
		}

	/*
		SERIALISE()
		-----------
	*/
	func serialise(file: FileHandle)
		{
		file.write(Data(bytes: &caps, count:MemoryLayout.size(ofValue:caps)))
		file.write(Data(bytes: &shift, count:MemoryLayout.size(ofValue:shift)))
		file.write(Data(bytes: &control, count:MemoryLayout.size(ofValue:control)))
		}

	/*
		DESERIALISE()
		-----------
	*/
	func deserialise(file: FileHandle) throws
		{
		var data = try file.read(upToCount: MemoryLayout.size(ofValue:caps))
		caps = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: caps).self)})

		data = try file.read(upToCount: MemoryLayout.size(ofValue:shift))
		shift = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: shift).self)})

		data = try file.read(upToCount: MemoryLayout.size(ofValue:control))
		control = data!.withUnsafeBytes({(rawPtr: UnsafeRawBufferPointer) in return rawPtr.load(as: type(of: control).self)})
		}
	}

