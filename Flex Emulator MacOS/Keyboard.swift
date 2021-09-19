/*
	KEYBOARD.SWIFT
	--------------
*/
import AppKit
import SwiftUI
import Foundation
import CoreGraphics

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
		----------=---
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
					let flex_key = ascii == 0x7F ? 0x08 : CChar(ascii)
					machine_queue_key_press(machine.pointer, flex_key)
					}
				else
					{
					let character = Int(event.keyCode)
					let key_arrrow_left  = 123
					let key_arrow_right = 124
					let key_arrow_down = 125
					let key_arrow_up = 126
					switch character
						{
						case key_arrrow_left:
							machine_queue_key_press(machine.pointer, 0x08)
						case key_arrow_right:
							machine_queue_key_press(machine.pointer, 0x09)
						case key_arrow_down:
							machine_queue_key_press(machine.pointer, 0x0A)
						case key_arrow_up:
							machine_queue_key_press(machine.pointer, 0x0B)
						default:
							break
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
