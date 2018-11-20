//
//  MenuCell.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/16/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

import UIKit

struct CellModel {
    let image : UIImage
    let name  : String
}

class MenuCell: UICollectionViewCell {
    
    @IBOutlet var cellImage: UIImageView!
//    @IBOutlet var cellLabel: UILabel!
    @IBOutlet weak var cellButton: UIButton!
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        self.backgroundColor = UIColor(red: 0.3, green: 0.3, blue: 0.3, alpha: 1.0)
        self.layer.cornerRadius = 10.0
        
//        cellLabel = UILabel(frame: CGRect(x: 0, y: frame.size.height, width: frame.width, height: frame.size.height * 0.3))
//        cellLabel.text = "Hello"
//        addSubview(cellLabel)
    }
    
    @IBAction func cellClicked(_ sender: UIButton) {
        print("Touched") //not ever printing on click
    }
  
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}
