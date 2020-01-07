//
//  Quadtree.m
//  FieldAlchemy
//
//  Created by Bryan Tremblay on 11/6/14.
//  Copyright (c) 2014 Bryan Tremblay. All rights reserved.
//

#import "Quadtree.h"
#import "GridSquare.h"

@implementation Quadtree
@synthesize parentNode, northWest, northEast, southWest, southEast, squares, boundingBox, capacity;

- (id)initWithBoundingBox:(CGRect)_boundingBox parentNode:(Quadtree *)_parentNode {
	if(self = [super init]) {
		self.parentNode = _parentNode;
		self.boundingBox = _boundingBox;
		self.capacity = 1;
		self.squares = [NSMutableArray array];
	}
	return self;
}

- (NSMutableArray *)allBoundingBoxes {
	NSMutableArray *retVal = [NSMutableArray array];

	[retVal addObject:[NSValue valueWithCGRect:boundingBox]];

	if(northWest != NULL)[retVal addObjectsFromArray:[northWest allBoundingBoxes]];
	if(northEast != NULL)[retVal addObjectsFromArray:[northEast allBoundingBoxes]];
	if(southWest != NULL)[retVal addObjectsFromArray:[southWest allBoundingBoxes]];
	if(southEast != NULL)[retVal addObjectsFromArray:[southEast allBoundingBoxes]];

	return retVal;
}

- (NSMutableArray *)allSquares {
	NSMutableArray *retVal = [NSMutableArray array];

	[retVal addObjectsFromArray:squares];

	if(northWest != NULL)[retVal addObjectsFromArray:[northWest allSquares]];
	if(northEast != NULL)[retVal addObjectsFromArray:[northEast allSquares]];
	if(southWest != NULL)[retVal addObjectsFromArray:[southWest allSquares]];
	if(southEast != NULL)[retVal addObjectsFromArray:[southEast allSquares]];

	return retVal;
}

- (void)subdivide {
	northWest = [[Quadtree alloc] initWithBoundingBox:CGRectMake(boundingBox.origin.x, boundingBox.origin.y, boundingBox.size.width/2, boundingBox.size.height/2) parentNode:self];
	northEast = [[Quadtree alloc] initWithBoundingBox:CGRectMake(boundingBox.origin.x + boundingBox.size.width/2, boundingBox.origin.y, boundingBox.size.width/2, boundingBox.size.height/2) parentNode:self];
	southWest = [[Quadtree alloc] initWithBoundingBox:CGRectMake(boundingBox.origin.x, boundingBox.origin.y + boundingBox.size.height/2, boundingBox.size.width/2, boundingBox.size.height/2) parentNode:self];
	southEast = [[Quadtree alloc] initWithBoundingBox:CGRectMake(boundingBox.origin.x + boundingBox.size.width/2, boundingBox.origin.y + boundingBox.size.height/2, boundingBox.size.width/2, boundingBox.size.height/2) parentNode: self];
}

- (BOOL)add:(GridSquare *)square {
	if(!CGRectContainsPoint(boundingBox, square.position)) {
		return false;
	}

	if(squares.count < capacity) {
		[squares addObject:square];
		return true;
	}

	if(northWest == NULL) {
		[self subdivide];
	}

	if([northWest add:square])return true;
	if([northEast add:square])return true;
	if([southWest add:square])return true;
	if([southEast add:square])return true;

	return false;
}

- (BOOL)remove:(GridSquare *)square {
	if([squares containsObject:square]) {
		[squares removeObject:square];
		return true;
	}

	if(northWest) {
		if([northWest remove:square])return true;
	}
	if(northEast) {
		if([northEast remove:square])return true;
	}
	if(southWest) {
		if([southWest remove:square])return true;
	}
	if(southEast) {
		if([southEast remove:square])return true;
	}

	return false;
}

- (GridSquare *)squareAt:(CGPoint)location {
	if(northWest != NULL) {
		GridSquare *retVal = [northWest squareAt:location];
		if(retVal != NULL)return retVal;
	}
	if(northEast != NULL) {
		GridSquare *retVal = [northEast squareAt:location];
		if(retVal != NULL)return retVal;
	}
	if(southWest != NULL) {
		GridSquare *retVal = [southWest squareAt:location];
		if(retVal != NULL)return retVal;
	}
	if(southEast != NULL) {
		GridSquare *retVal = [southEast squareAt:location];
		if(retVal != NULL)return retVal;
	}

	for(GridSquare *square in squares) {
		if(CGRectContainsPoint(square.frame, location))
			return square;
	}

	return NULL;
}

- (void)render_DEBUG:(SKScene *)scene {
	NSMutableArray *allBoundingBoxes = [self allBoundingBoxes];
	[self removeRenderBoxes_DEBUG];
	renderBoxes_DEBUG = [NSMutableArray array];
	
	for(NSValue *_boundingBox in allBoundingBoxes) {
		CGRect boundingRect = _boundingBox.CGRectValue;
		SKShapeNode *box = [SKShapeNode shapeNodeWithRect:boundingRect];
		box.strokeColor = [UIColor redColor];
		box.userInteractionEnabled = NO;
		[scene addChild:box];
		[renderBoxes_DEBUG addObject:box];
	}
}

- (void)removeRenderBoxes_DEBUG {
	if(renderBoxes_DEBUG != NULL) {
		for(SKShapeNode *box in renderBoxes_DEBUG)
			[box removeFromParent];
	}
}

@end