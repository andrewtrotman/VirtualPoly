/*
	KEYBOARD.SWIFT
	--------------
*/
import AppKit
import SwiftUI
import Foundation
import CoreGraphics
import Carbon.HIToolbox

/*
	STRUCT KEY_EVENT_HANDLING
	-------------------------
	Keyboard management for SwiftUI on the Mac.
	Addapted from: https://stackoverflow.com/questions/61153562/how-to-detect-keyboard-events-in-swiftui-on-macos
*/
struct key_event_handling: NSViewRepresentable
	{
	@Binding var machine: machine_changer

	/*
		CLASS KEY_VIEW
		--------------
	*/
	class key_view: NSView
		{
		var machine: machine_changer

		/*
			ACCEPTSFIRSTRESPONDER
			---------------------
		*/
		override var acceptsFirstResponder: Bool
			{
			true
			}

		/*
			KEYDOWN()
			---------
		*/
		override func keyDown(with event: NSEvent)
			{
			if event.characters?.count == 1
				{
				if let ascii = Character(event.characters!).asciiValue
					{
					var flex_key = CChar(ascii)
					if AppState.shared.emulated_machine == PINNATED
						{
						if (ascii == 0x7F)
							{
							flex_key = 0x08
							}
						}
					else if AppState.shared.emulated_machine == POLY_1
						{
						switch ascii
							{
							case Character("@").asciiValue:
								flex_key = CChar(poly_key_at)
							case Character("|").asciiValue:
								flex_key = CChar(poly_key_bar)
							case Character("#").asciiValue:
								flex_key = CChar(poly_key_pound)
							case Character("^").asciiValue:
								flex_key = CChar(poly_key_exp)
/*
	These ones don't work - debug the Poly BIOS to see why not.
*/
							case Character("}").asciiValue:
								flex_key = CChar(125)
							case Character("{").asciiValue:
								flex_key = CChar(123)
							case Character("[").asciiValue:
								flex_key = CChar(91)
							case Character("]").asciiValue:
								flex_key = CChar(93)
							default:
								break
							}
						}

					machine_queue_key_press(machine.pointer, flex_key)
					}
				else
					{
					let character = Int(event.keyCode)
					switch character
						{
						case kVK_LeftArrow:
							machine_queue_key_press(machine.pointer, CChar(poly_key_left))
						case kVK_RightArrow:
							machine_queue_key_press(machine.pointer, CChar(poly_key_right))
						case kVK_DownArrow:
							machine_queue_key_press(machine.pointer, CChar(poly_key_down))
						case kVK_UpArrow:
							machine_queue_key_press(machine.pointer, CChar(poly_key_up))
						case kVK_ANSI_Keypad0:
							machine_queue_key_press(machine.pointer, CChar(poly_key_u0))
						case kVK_ANSI_Keypad1:
							machine_queue_key_press(machine.pointer, CChar(poly_key_u1))
						case kVK_ANSI_Keypad2:
							machine_queue_key_press(machine.pointer, CChar(poly_key_u2))
						case kVK_ANSI_Keypad3:
							machine_queue_key_press(machine.pointer, CChar(poly_key_u3))
						case kVK_ANSI_Keypad4:
							machine_queue_key_press(machine.pointer, CChar(poly_key_u4))
						case kVK_ANSI_Keypad5:
							machine_queue_key_press(machine.pointer, CChar(poly_key_u5))
						case kVK_ANSI_Keypad6:
							machine_queue_key_press(machine.pointer, CChar(poly_key_u6))
						case kVK_ANSI_Keypad7:
							machine_queue_key_press(machine.pointer, CChar(poly_key_u7))
						case kVK_ANSI_Keypad8:
							machine_queue_key_press(machine.pointer, CChar(poly_key_u8))
						case kVK_ANSI_Keypad9:
							machine_queue_key_press(machine.pointer, CChar(poly_key_u9))
						case kVK_ANSI_KeypadDecimal:
							machine_queue_key_press(machine.pointer, CChar(poly_key_keypad_dot))
						case kVK_Help:
							machine_queue_key_press(machine.pointer, CChar(poly_key_help))
						default:
							break

//	poly_key_line_insert = 0x7B;
//	poly_key_char_insert = 0x5B;
//	poly_key_line_del = 0x5E;
//	poly_key_char_del = 0x7E;
//	poly_key_calc = 0x12;
//	poly_key_exit = 0x5C;
//	poly_key_back = 0x7C;
//	poly_key_repeat = 0x7D;
//	poly_key_next = 0x5D;
//	poly_key_pause = 0x0F;
//	poly_key_shift_pause = 0x0E;



						}
					}
				}
			}

		/*
			INIT()
			------
		*/
		init(machine: machine_changer)
			{
			self.machine = machine
			super.init(frame: .zero)
			}

		/*
			INIT()
			------
		*/
		required init?(coder: NSCoder)
			{
			fatalError("init(coder:) has not been implemented")
			}
	}

	/*
		MAKENSVIEW()
		------------
	*/
	func makeNSView(context: Context) -> NSView
		{
		let view = key_view(machine: machine)
		DispatchQueue.main.async
			{
			view.window?.makeFirstResponder(view)
			}
		return view
		}

	/*
		UPDATENSVIEW()
		--------------
	*/
	func updateNSView(_ nsView: NSView, context: Context)
		{
		}
	}
