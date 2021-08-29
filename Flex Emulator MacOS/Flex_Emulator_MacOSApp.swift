/*
	FLEX_EMULATOR_MACOSAPP.SWIFT
	----------------------------
*/
import SwiftUI
import Combine

@main
struct Flex_Emulator_MacOSApp: App
	{
    private let keyInputSubject = KeyInputSubjectWrapper()

	var body: some Scene
		{
		WindowGroup
			{
			ContentView()
                .frame(idealWidth: .infinity, idealHeight: .infinity)
				.onReceive(keyInputSubject)
					{
					print("Key pressed: \($0)")
					}
				.environmentObject(keyInputSubject)
			}
			.commands
				{
				CommandMenu("Input")
					{
					keyInput(.leftArrow)
					keyInput(.rightArrow)
					keyInput(.upArrow)
					keyInput(.downArrow)
					keyInput(.space)
					}
				}
			}
	}

/*
	Keyboard management in SwiftUI is from here:
	https://stackoverflow.com/questions/61153562/how-to-detect-keyboard-events-in-swiftui-on-macos
*/
public extension EventModifiers
	{
	static let none = Self()
	}

public typealias KeyInputSubject = PassthroughSubject<KeyEquivalent, Never>


public final class KeyInputSubjectWrapper: ObservableObject, Subject
	{
	public func send(_ value: Output)
		{
		objectWillChange.send(value)
		}

	public func send(completion: Subscribers.Completion<Failure>)
		{
		objectWillChange.send(completion: completion)
		}

	public func send(subscription: Subscription)
		{
		objectWillChange.send(subscription: subscription)
		}

	public typealias ObjectWillChangePublisher = KeyInputSubject

	public let objectWillChange: ObjectWillChangePublisher

	public init(subject: ObjectWillChangePublisher = .init())
		{
		objectWillChange = subject
		}
	}

public extension KeyInputSubjectWrapper
	{
	typealias Output = KeyInputSubject.Output
	typealias Failure = KeyInputSubject.Failure

	func receive<S>(subscriber: S) where S : Subscriber, S.Failure == Failure, S.Input == Output
		{
		objectWillChange.receive(subscriber: subscriber)
		}
	}

private extension Flex_Emulator_MacOSApp
	{
	func keyInput(_ key: KeyEquivalent, modifiers: EventModifiers = .none) -> some View
		{
		keyboardShortcut(key, sender: keyInputSubject, modifiers: modifiers)
		}
	}

public func keyboardShortcut<Sender, Label>
	(
    _ key: KeyEquivalent,
    sender: Sender,
    modifiers: EventModifiers = .none,
    @ViewBuilder label: () -> Label
	) -> some View where Label: View, Sender: Subject, Sender.Output == KeyEquivalent
	{
    Button(action: { sender.send(key) }, label: label)
        .keyboardShortcut(key, modifiers: modifiers)
	}


public func keyboardShortcut<Sender>
	(
    _ key: KeyEquivalent,
    sender: Sender,
    modifiers: EventModifiers = .none
	) -> some View where Sender: Subject, Sender.Output == KeyEquivalent
	{

    guard let nameFromKey = key.name else
    	{
        return AnyView(EmptyView())
    	}
    return AnyView(keyboardShortcut(key, sender: sender, modifiers: modifiers)
    	{
        Text("\(nameFromKey)")
    	}
	)
}







extension KeyEquivalent: Equatable {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        lhs.character == rhs.character
    }
}

extension String {
    func capitalizingFirstLetter() -> String {
      return prefix(1).uppercased() + self.lowercased().dropFirst()
    }

    mutating func capitalizeFirstLetter() {
      self = self.capitalizingFirstLetter()
    }
}


extension KeyEquivalent {
    var lowerCaseName: String? {
        switch self {
        case .space: return "space"
        case .clear: return "clear"
        case .delete: return "delete"
        case .deleteForward: return "delete forward"
        case .downArrow: return "down arrow"
        case .end: return "end"
        case .escape: return "escape"
        case .home: return "home"
        case .leftArrow: return "left arrow"
        case .pageDown: return "page down"
        case .pageUp: return "page up"
        case .return: return "return"
        case .rightArrow: return "right arrow"
        case .space: return "space"
        case .tab: return "tab"
        case .upArrow: return "up arrow"
        default: return nil
        }
    }

    var name: String? {
        lowerCaseName?.capitalizingFirstLetter()
    }
}
