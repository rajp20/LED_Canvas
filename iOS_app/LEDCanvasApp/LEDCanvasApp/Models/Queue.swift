//
//  Queue.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/17/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

public struct Queue<T> : LineProtocol{
    var line: Line
    
    fileprivate var q    : [Any]!
    fileprivate var size : Int!
    
    public init(queue: [Any]? = nil, count: Int? = nil, _line: Line? = nil) {
        self.q    = [Any]()
        self.size = 0
        self.line = _line ?? Line(lineAt: ["from": CGPoint(x: 0.0, y: 0.0), "to" : CGPoint(x: 0.0, y: 0.0)], alphaValue: 0.0)
//        self.pixel = _pixel ?? Pixel(pointAt: CGPoint(x:0.0, y:0.0), alphaValue: 0.0)
    }
    
    public mutating func enqueue<T>(_ element: T) {
        size += 1
        q.append(element)
    }
    
    public mutating func dequeue<T>() -> T? where T : Line{
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
    
    public func peek<T>() -> T where T : Line{
        return q.first as! T
    }
    
    public func count() -> Int {
        return size
    }
    
    public func list<T>() -> [T]? where T : Line{
        if self.size == 0 {
            return []
        }
        return q as? [T]
    }
    
    public mutating func clearQueue() {
        q = [Any]()
    }
    
    public mutating func updateQueue(weight: CGFloat) {
        
        if !self.isEmpty() {
            while self.size != 0 && self.peek().alpha <= 0 {
                self.popTop()
            }
            if !self.isEmpty() {
                if self.size > 0 {
                    for i in 0...self.size - 1 {
                        guard let line = q[i] as? Line else { return }
                        line.alpha -= weight
                    }
                }
            }
        }
    }
}

/********* Protocol for Pixel Class *********/
protocol LineProtocol {
    var line: Line { get set }
}

/********* Extension to print contents of the Queue object *********/

extension Queue : CustomStringConvertible {
    
    public var description : String {
        return q.description
    }
}
