/*
	COMPATIBILITY.SWIFT
	-------------------
	Taken from: https://www.swiftbysundell.com/tips/making-uiimage-macos-compatible/
*/
import Cocoa

typealias UIImage = NSImage

/*
	EXTENSION NSIMAGE
	-----------------
*/
extension NSImage
	{
	var cgImage: CGImage?
		{
		var proposedRect = CGRect(origin: .zero, size: size)

		return cgImage(forProposedRect: &proposedRect, context: nil, hints: nil)
		}
	}
