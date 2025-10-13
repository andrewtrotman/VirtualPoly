/*
	FLEX_EMULATOR_MACOSAPP.SWIFT
	----------------------------
*/
import SwiftUI

class AppState: ObservableObject
	{
	static let shared = AppState()

	var machine: UnsafeRawPointer? = nil
	var screen: screen_base? = nil
//	let emulated_machine = ARROW
//	let emulated_machine = PINNATED
	let emulated_machine = POLY_1
//	let emulated_machine = PROTEUS
//	let emulated_machine = POLY_WITH_PROTEUS
	}

class AppDelegate: NSObject, NSApplicationDelegate
	{
	func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool
		{
		return true
		}
	}

@main
struct Flex_Emulator_MacOSApp: App
	{
    @NSApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
    @Environment(\.openURL) var openURL
    @StateObject var app_state = AppState.shared
    @State var clipboard_has_text = true
    @State var text_selected = true
    @State var acknowledgements_open = false
    @State var disk_0: String = "Flex"
    @State var disk_1: String = "User"
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
					machine_destruct(AppState.shared.machine)
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
//				CommandGroup(after: .saveItem)
				CommandMenu("6809")
					{
					file_menu(disk_0_name: $disk_0, disk_1_name: $disk_1)
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
//		Button("Select All")
//			{
//
//			}
//			.keyboardShortcut("a")
		}
	}

struct file_menu: View
	{
	@Binding var disk_0_name: String
	@Binding var disk_1_name: String

	var body: some View
		{
		Button("Drive 0: " + disk_0_name)
			{
			if let got = showOpenPanel()
				{
				let c_name = machine_change_disk(AppState.shared.machine, 0, got)
				disk_0_name = String(cString: c_name!)
				}
			}
		Button("Drive 1: " + disk_1_name)
			{
			if let got = showOpenPanel()
				{
				let c_name = machine_change_disk(AppState.shared.machine, 1, got)
				disk_1_name = String(cString: c_name!)
				}
			}
		}
	}

func showOpenPanel() -> String?
	{
	let openPanel = NSOpenPanel()
	openPanel.allowedFileTypes = ["dsk", "DSK"]
	openPanel.allowsMultipleSelection = false
	openPanel.canChooseDirectories = false
	openPanel.canChooseFiles = true
	let response = openPanel.runModal()
	return response == .OK ? openPanel.url?.path : nil
	}

