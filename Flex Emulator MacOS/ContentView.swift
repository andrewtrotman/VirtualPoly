/*
	CONTENTVIEW.SWIFT
	-----------------
	Copyright (c) 2021 Andrew Trotman
*/
import SwiftUI
import Foundation
import CoreGraphics

/*
	CLASS IMAGE_CHANGER
	-------------------
*/
class image_changer: ObservableObject
	{
	let offscreen_bitmap = CGContext(data: malloc(480 * 240 * 4), width: 480, height: 240, bitsPerComponent: 8, bytesPerRow: 480 * 4, space: CGColorSpaceCreateDeviceRGB(), bitmapInfo: CGImageAlphaInfo.noneSkipLast.rawValue)!
	@Published public var image = UIImage(named: "480x240")!
	public var frame_buffer: UnsafeMutablePointer<UInt8>?

	/*
		INIT()
		------
	*/
	init()
		{
		frame_buffer = offscreen_bitmap.data!.assumingMemoryBound(to: UInt8.self)
		}
	}

/*
	CLASS MACHINE_CHANGER
	---------------------
*/
class machine_changer: ObservableObject
	{
	@Published public var pointer: UnsafeRawPointer? = nil
	}

/*
	STRUCT CONTENTVIEW
	------------------
*/
struct ContentView: View
	{
	@StateObject var app_state : AppState

	static let CPU_speed: Double = 20000000			// 1,000,000 is 1 MHz
	static let iOS_timer_hz: Double = 25		// interrupts per second

	@State var flash_timer = Timer.publish(every: 0.5, on: .main, in: .common).autoconnect()
	@State var cpu_timer = Timer.publish(every: 1.0 / ContentView.iOS_timer_hz, on: .main, in: .common).autoconnect()
	@State var one_second_timer = Timer.publish(every: 1.0, on: .main, in: .common).autoconnect()

	@State var machine = machine_changer()
	@State var paused = false							// the 6809 is paused

	@State var screen: screen_base? = nil

	@StateObject var img_screen = image_changer()

	@State var initial_time = NSDate()
	@State var previous_time = NSDate()
	@State var previous_cycle_count: UInt64 = 0

	/*
		RESET()
		-------
	*/
	func reset()
		{
		previous_cycle_count = 0
		paused = false;
		if machine.pointer != nil
			{
			machine_reset(machine.pointer);
			}
		screen!.reset()
		screen!.set_width(new_width: terminal.screen_width.eighty)
		}

	/*
		START_TIMERS()
		--------------
	*/
	func start_timers()
		{
		one_second_timer = Timer.publish(every: 1.0, on: .main, in: .common).autoconnect()
		flash_timer = Timer.publish(every: 0.5, on: .main, in: .common).autoconnect()
		cpu_timer = Timer.publish(every: 1.0 / ContentView.iOS_timer_hz, on: .main, in: .common).autoconnect()
		}

	/*
		STOP_TIMERS()
		-------------
	*/
	func stop_timers()
		{
		cpu_timer.upstream.connect().cancel()
		flash_timer.upstream.connect().cancel()
		one_second_timer.upstream.connect().cancel()
		}

	/*
		BODY
		----
	*/
	var body: some View
		{
		VStack
			{
			Image(nsImage: img_screen.image)
				.resizable()
				.onAppear(perform:
					{
					render_text_screen()
					})
				.background(key_event_handling(machine: $machine))

			Spacer()
				.frame(maxHeight: 0)
				.onReceive(cpu_timer)
					{ _ in
					if (!paused)
						{
						var screen_did_change = false

                  let total_seconds_count = -initial_time.timeIntervalSinceNow
						let end_cycle = UInt64(ContentView.CPU_speed * total_seconds_count)

						if end_cycle > machine_cycles_spent(machine.pointer) + 10 * UInt64((ContentView.CPU_speed / ContentView.iOS_timer_hz))
							{
							machine_set_cycles_spent(machine.pointer, UInt64(Double(ContentView.CPU_speed) * total_seconds_count) - UInt64((ContentView.CPU_speed / ContentView.iOS_timer_hz)))
							}

						while (machine_cycles_spent(machine.pointer) < end_cycle)
							{
							machine_step(machine.pointer, UInt64(ContentView.CPU_speed / ContentView.iOS_timer_hz) / 8);

							var response = machine_dequeue_serial_output(machine.pointer)
							while (response <= 0xFF)
								{
								screen!.print_character(raw_character: UInt8(response & 0xFF))

//print(Character(UnicodeScalar(UInt8(response))), terminator:"")
								screen_did_change = true
								response = machine_dequeue_serial_output(machine.pointer)
								}

							if screen_did_change || machine_did_screen_change(machine.pointer)
								{
								render_text_screen()
								break;
								}
							}
						}
					}
				.onReceive(flash_timer)
					{ _ in
					if !paused
						{
						screen!.flash_state.toggle()
						render_text_screen()
						}
					}
				.onReceive(one_second_timer)
					{ _ in
//					let total_cycles_spent: UInt64 = UInt64(machine_cycles_spent(machine.pointer))
//					let slice_cycles_spent: UInt64 = total_cycles_spent - previous_cycle_count
//
//					let total_seconds_count = -initial_time.timeIntervalSinceNow
//					let current_seconds_count = -previous_time.timeIntervalSinceNow
//
//					print("total=\((Double(total_cycles_spent) / total_seconds_count / 1000.0).rounded()) KHz now=\((Double(slice_cycles_spent) / current_seconds_count / 1000.0).rounded()) KHz \n");
//
//					previous_time = NSDate()
//					previous_cycle_count = machine_cycles_spent(machine.pointer)
					}
				.onAppear
					{
					if (machine.pointer == nil)
						{
						if (AppState.shared.emulated_machine == ARROW)
							{
							machine.pointer = machine_construct(ARROW)
							screen = screen_arrow()
							}
						else if (AppState.shared.emulated_machine == PINNATED)
							{
							machine.pointer = machine_construct(PINNATED)
//							screen = screen_pinnated()
							screen = terminal()
							}
						else	// must be a Poly emulator
							{
							machine.pointer = machine_construct(POLY_1)
							let poly_screen = screen_poly_1()
							poly_screen.set_machine(poly: machine.pointer)
							screen = poly_screen
							}

						screen!.set_screen_buffer(screen_buffer: machine_get_screen_buffer(machine.pointer))
						AppState.shared.machine = machine.pointer
						AppState.shared.screen = screen

						stop_timers()
						initial_time = NSDate()
						previous_time = NSDate()
						reset()
						start_timers()
						}
					}
		}
		.background(Color.black)
	}

	/*
		RENDER_TEXT_SCREEN()
		--------------------
	*/
	func render_text_screen()
		{
		screen!.render_entire_screen()
		memcpy(img_screen.frame_buffer, screen!.bitmap, 480 * 240 * 4)
		img_screen.image = UIImage(cgImage: img_screen.offscreen_bitmap.makeImage()!, size: .zero)
		}
	}
