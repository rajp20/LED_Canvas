//
//  Queue.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/17/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

public struct Queue<T> : PixelProtocol{
    var pixel: Pixel
    
    fileprivate var q    : [Any]!
    fileprivate var size : Int!
    
    public init(queue: [Any]? = nil, count: Int? = nil, _pixel: Pixel? = nil) {
        self.q    = [Any]()
        self.size = 0
        self.pixel = _pixel ?? Pixel(pointAt: CGPoint(x:0.0, y:0.0), alphaValue: 0.0)
    }
    
    public mutating func enqueue<T>(_ element: T) {
        size += 1
        q.append(element)
    }
    
    public mutating func dequeue<T>() -> T? where T : Pixel{
        size -= 1
        guard !q.isEmpty, let element = q.first else { return nil }
        
        q.removeFirst()
        
        return element as? T
    }
    
    private mutating func popTop() {
        size -= 1
        q.removeFirst()
    }
    
    public func isEmpty() -> Bool {
        return q.isEmpty
    }
    
    public func peek<T>() -> T where T : Pixel{
        return q.first as! T
    }
    
    public func count() -> Int {
        return size
    }
    
    public func list<T>() -> [T]? where T : Pixel{
        if self.size == 0 {
            return []
        }
        return q as? [T]
    }
    
    public mutating func updateQueue(weight: CGFloat) {
        
        if !self.isEmpty() {
            if self.peek().alpha <= 0.0 {
                self.popTop()
            }
            
            if self.size > 0 {
                for i in 0...self.size - 1 {
                    guard let pixel = q[i] as? Pixel else { return }
                    pixel.alpha -= weight
                    print(pixel.alpha)
                }
            }
        }
    }
}

/********* Protocol for Pixel Class *********/
protocol PixelProtocol {
    var pixel: Pixel { get set }
}

/********* Extension to print contents of the Queue object *********/

extension Queue : CustomStringConvertible {
    
    public var description : String {
        return q.description
    }
}
