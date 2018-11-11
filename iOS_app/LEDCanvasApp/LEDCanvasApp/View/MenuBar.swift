//
//  MenuBar.swift
//  LEDCanvasApp
//
//  Created by Blaze Kotsenburg on 11/11/18.
//  Copyright © 2018 Blaze Kotsenburg. All rights reserved.
//

/**
 * This class is the custom menu bar class used to hold all of the options available for the user to draw with at the bottom of the
 * screen.
 */
import UIKit

class MenuBar : UIView, UICollectionViewDelegate, UICollectionViewDataSource {
    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return 4
    }
    
    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: cellID, for: indexPath)
        return cell
    }
    
    lazy var collectionView : UICollectionView = {
        let layout = UICollectionViewFlowLayout()
        let cv = UICollectionView(frame: .zero, collectionViewLayout: layout)
        cv.dataSource = self
        cv.delegate   = self
        return cv
    }()
    
    let cellID = "cellID"
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        collectionView.register(UICollectionViewCell.self, forCellWithReuseIdentifier: cellID)
        
        addSubview(collectionView)
        collectionView.backgroundColor = UIColor.cyan
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}
