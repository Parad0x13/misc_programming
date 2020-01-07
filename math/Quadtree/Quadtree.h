//
//  Quadtree.h
//  FieldAlchemy
//
//  Created by Bryan Tremblay on 11/6/14.
//  Copyright (c) 2014 Bryan Tremblay. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <SpriteKit/SpriteKit.h>

@class GridSquare;

@interface Quadtree : NSObject {
	NSMutableArray *renderBoxes_DEBUG;
}

@property(nonatomic, weak)Quadtree *parentNode;		// The base node will have parentNode = NULL
@property(nonatomic, strong)Quadtree *northWest, *northEast, *southWest, *southEast;
@property(nonatomic, strong)NSMutableArray *squares;
@property(nonatomic)CGRect boundingBox;
@property(nonatomic)int capacity;

- (id)initWithBoundingBox:(CGRect)_boundingBox parentNode:(Quadtree *)_parentNode;
- (NSMutableArray *)allBoundingBoxes;
- (NSMutableArray *)allSquares;

- (void)subdivide;
- (BOOL)add:(GridSquare *)square;
- (BOOL)remove:(GridSquare *)square;
///- (void)prune;

- (GridSquare *)squareAt:(CGPoint)location;

- (void)render_DEBUG:(SKScene *)scene;
- (void)removeRenderBoxes_DEBUG;

@end
