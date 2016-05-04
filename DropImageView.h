//
//  DropImageView.h
//  OpenCVSkeletonGUI
//
//  Created by Vitaly Ishkulov on 10/12/13.
//  Copyright (c) 2013 Vitaly Ishkulov. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "NSImage+OpenCV.h"
#import "SkeletonLib.h"
#import "ContoursFinder.h"

@interface DropImageView : NSImageView </* NSDraggingSource, */ NSDraggingDestination, NSPasteboardItemDataProvider > {
    BOOL highlight;
    BOOL ended;
}

- (id)initWithCoder:(NSCoder *)aDecoder;

@end
