//
//  DropImageView.m
//  OpenCVSkeletonGUI
//
//  Created by Vitaly Ishkulov on 10/12/13.
//  Copyright (c) 2013 Vitaly Ishkulov. All rights reserved.
//

#import "DropImageView.h"
//#import <OpenCV2/opencv.hpp>

@interface DropImageView ()
- (NSRect)resizeImageBounds:(NSImageView *)container withSize:(int)maxSize;
@end

@implementation DropImageView

- (NSRect)resizeImageBounds:(NSImageView *)container withSize:(int)maxSize {
    
    NSRect draggedImageRect; //Getting image bounds to make it nice looking during dragging out
    draggedImageRect.size.width = container.image.size.width;
    draggedImageRect.size.height = container.image.size.height;
    
    if (draggedImageRect.size.width > maxSize && container.image.size.width >= container.image.size.height) {
        draggedImageRect.size.width = maxSize;
        draggedImageRect.size.height = container.image.size.height/container.image.size.width*maxSize;
    } else {
        draggedImageRect.size.height = maxSize;
        draggedImageRect.size.width = container.image.size.width/container.image.size.height*maxSize;
    }
    
    return draggedImageRect;
}

NSString *kPrivateDragUTI = @"com.IVA.OpenCVSkeletonGUI";

- (id)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    [self setImageScaling:NSScaleNone]; // Initially don't scale image
    if (self) {
        [self registerForDraggedTypes:[NSImage imagePasteboardTypes]];
    }
    return self;
}

- (NSDragOperation)draggingEntered:(id<NSDraggingInfo>)sender {
    if ([NSImage canInitWithPasteboard:[sender draggingPasteboard]] && [sender draggingSourceOperationMask] & NSDragOperationCopy) {
        highlight = YES;
        
        [self setNeedsDisplay: YES];
        
        //NSRect containedImageRect = [self resizeImageBounds:self withSize:600];
        
        
        [sender enumerateDraggingItemsWithOptions:NSDraggingItemEnumerationConcurrent
                                          forView:self
                                          classes:[NSArray arrayWithObject:[NSPasteboardItem class]]
                                    searchOptions:nil
                                       usingBlock:^(NSDraggingItem *draggingItem, NSInteger idx, BOOL *stop) {
                                           if (![[[draggingItem item] types] containsObject:kPrivateDragUTI]) {
                                               *stop = YES;
                                           } else {
                                               [draggingItem setDraggingFrame: self.bounds contents:[[[draggingItem imageComponents] objectAtIndex:0]contents]];
                                           }
        }];
        return NSDragOperationCopy;
    }
    return NSDragOperationNone;
}

- (void)draggingExited:(id<NSDraggingInfo>)sender {
    highlight = NO;
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect
{
	[super drawRect:dirtyRect];
	
    if (highlight) {
        [[NSColor blueColor] set];
        [NSBezierPath setDefaultLineWidth:5];
        [NSBezierPath strokeRect:dirtyRect];
    }
}

- (BOOL)prepareForDragOperation:(id<NSDraggingInfo>)sender {
    highlight = NO;
    
    [self setNeedsDisplay:YES];
    
    return [NSImage canInitWithPasteboard:[sender draggingPasteboard]];
}

- (BOOL)performDragOperation:(id<NSDraggingInfo>)sender {
    if ([sender draggingSource] != self) {
        NSURL *fileURL;
        
        if ([NSImage canInitWithPasteboard:[sender draggingPasteboard]]) {
            NSImage *newImage = [[NSImage alloc] initWithPasteboard:[sender draggingPasteboard]];
            
//            [self setImageScaling:NSScaleProportionally];
            [self setImageScaling:NSImageScaleProportionallyUpOrDown];
            [self setImage:newImage];
            
        }
        fileURL = [NSURL URLFromPasteboard:[sender draggingPasteboard]];
        [[self window] setTitle: fileURL != NULL ? [fileURL lastPathComponent] : @"Untitled" ];
    }
    return YES;
}

- (NSRect)windowWillUseStandardFrame:(NSWindow *)window defaultFrame:(NSRect)newFrame; {
    NSRect ContentRect = self.window.frame;
    
    ContentRect.size = [[self image] size];
    
    return [NSWindow frameRectForContentRect:ContentRect styleMask:[window styleMask]];
};

// Dragging out
/*
- (void)mouseDown:(NSEvent *)theEvent {
    NSPasteboardItem *pbItem = [NSPasteboardItem new];
    [pbItem setDataProvider:self forTypes:[NSArray arrayWithObjects:NSPasteboardTypeTIFF, NSPasteboardTypePDF, kPrivateDragUTI, nil]];
    NSDraggingItem *dragItem = [[NSDraggingItem alloc] initWithPasteboardWriter:pbItem];
    
    //NSRect draggingRect = [self resizeImageBounds:self withSize:300];
    
    [dragItem setDraggingFrame:self.bounds contents:[self image]];
    
    NSDraggingSession *draggingSession = [self beginDraggingSessionWithItems:[NSArray arrayWithObject:dragItem] event:theEvent source:self];
    
    draggingSession.animatesToStartingPositionsOnCancelOrFail = YES;
    
    draggingSession.draggingFormation = NSDraggingFormationNone;
}


- (NSDragOperation)draggingSession:(NSDraggingSession *)session sourceOperationMaskForDraggingContext:(NSDraggingContext)context {
    switch (context) {
        case NSDraggingContextOutsideApplication:
            return NSDragOperationCopy;
            
        case NSDraggingContextWithinApplication:
            
        default:
            return NSDragOperationCopy;
            break;
    }
}
*/

- (void)draggingEnded:(id <NSDraggingInfo>)sender {
    Mat initImage;
    NSImage *image = self.image; //getting dropped image
    initImage = [image CVMat]; //converting to OpenCV Matrix
    
    Mat result = drawShapes(initImage);
    NSImage *image2 = [NSImage imageWithCVMat:result];
    
    [self setImage:image2];
    
}

- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent {
    return YES;
}

- (void)pasteboard:(NSPasteboard *)pasteboard item:(NSPasteboardItem *)item provideDataForType:(NSString *)type {
    if ([type compare:NSPasteboardTypeTIFF] == NSOrderedSame) {
        [pasteboard setData:[[self image] TIFFRepresentation] forType:NSPasteboardTypeTIFF];
    } else if ([type compare:NSPasteboardTypePDF] == NSOrderedSame) {
        [pasteboard setData:[self dataWithEPSInsideRect:[self bounds]] forType:NSPasteboardTypePDF];
    }
}

@end











