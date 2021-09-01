/*
	FLEX_EMULATOR_MACOSAPP.SWIFT
	----------------------------
*/
import SwiftUI

class AppState: ObservableObject
	{
	static let shared = AppState()

	var machine: UnsafeRawPointer? = nil
	var screen: terminal? = nil
	}

@main
struct Flex_Emulator_MacOSApp: App
	{
    @StateObject var app_state = AppState.shared
    @State var clipboard_has_text = false
	@State var zoooom = 1.0
	@State var zoom = 1.0
	var body: some Scene
		{
		WindowGroup
			{
			ContentView(app_state: app_state)
				.onAppear
					{
					NSWindow.allowsAutomaticWindowTabbing = false
					}
				.onReceive(NotificationCenter.default.publisher(for: NSApplication.didBecomeActiveNotification))
					{ _ in
					clipboard_has_text = NSPasteboard.general.canReadItem(withDataConformingToTypes: ["public.plain-text"])
					}
				.onReceive(NotificationCenter.default.publisher(for: NSApplication.willTerminateNotification))
					{ _ in
//					machine_serialise(AppState.shared.machine)
					}
			}
			.commands
				{
				CommandGroup(replacing: .pasteboard)
					{
			//		Button("Copy"){}.keyboardShortcut("C").disabled(true)
					Button("Paste")
						{
						if let paste_buffer = NSPasteboard.general.pasteboardItems?.first?.string(forType: .string)
							{
							for char in paste_buffer
								{
								if let ascii = char.asciiValue
									{
									machine_queue_key_press(AppState.shared.machine, CChar(ascii))
									}
								}
							}
						}
						.keyboardShortcut("V").disabled(clipboard_has_text)
			//		Button("Select All"){}.keyboardShortcut("A").disabled(true)
					}
				CommandGroup(replacing: .undoRedo) {}
				CommandGroup(replacing: .sidebar) {}
				CommandGroup(replacing: .systemServices) {}
				CommandGroup(replacing: .newItem) {}
				CommandGroup(after: .saveItem)
					{
					Divider()
					Button("Reset")
						{
						print("Reset")
						machine_reset(AppState.shared.machine)
						AppState.shared.screen?.reset()
						AppState.shared.screen?.set_width(new_width: .eighty)
						}
					}
				}
		}
	}
