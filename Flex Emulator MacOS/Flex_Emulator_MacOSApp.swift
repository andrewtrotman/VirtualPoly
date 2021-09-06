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
    @Environment(\.openURL) var openURL
    @StateObject var app_state = AppState.shared
    @State var clipboard_has_text = true
    @State var text_selected = true
    @State var acknowledgements_open = false
    @State var disk_0: String = "Flex"
    @State var active = false


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
					cut_copy_paste_menu(can_cut: $text_selected, can_paste: $clipboard_has_text)
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
						machine_reset(AppState.shared.machine)
						AppState.shared.screen?.reset()
						AppState.shared.screen?.set_width(new_width: .eighty)
						}
					}

				CommandGroup(after: .help)
					{
					Divider()
					Button("Acknowledgements")
						{
						if !acknowledgements_open
							{
							acknowledgements_open = true
							if let url = URL(string: "Flex-Emulator-MacOS://Acknowledgements")
								{
								NSWorkspace.shared.open(url)
								}
							}
						}
					}
				}
		WindowGroup("Acknowledgements")
			{
			Acknowledgements(state: $acknowledgements_open)
			}.handlesExternalEvents(matching: Set(arrayLiteral: "Acknowledgements"))
		}
	}

struct cut_copy_paste_menu: View
	{
	@Binding var can_cut: Bool
	@Binding var can_paste: Bool

	var body: some View
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
			.disabled(!can_paste)
			.keyboardShortcut("v")
//		Button("Select All"){}.keyboardShortcut("A").disabled(true)
		}
	}
