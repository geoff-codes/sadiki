/*
	NSPrintOperation.h
	Application Kit
	Copyright (c) 1994-2003, Apple Computer, Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/AppKitDefines.h>

@class NSMutableData, NSGraphicsContext, NSPrintPanel, NSPrintInfo, NSView, NSWindow;

typedef enum _NSPrintingPageOrder {
    NSDescendingPageOrder		= (-1),
    NSSpecialPageOrder			= 0, // Tells spooler to not rearrange
    NSAscendingPageOrder		= 1,
    NSUnknownPageOrder			= 2  // No page order written out
} NSPrintingPageOrder;

/***** Exceptions *****/
APPKIT_EXTERN NSString *NSPrintOperationExistsException;


@interface NSPrintOperation : NSObject
{
}

+ (NSPrintOperation *)printOperationWithView:(NSView *)aView;
+ (NSPrintOperation *)printOperationWithView:(NSView *)aView printInfo:(NSPrintInfo *)aPrintInfo;

+ (NSPrintOperation *)EPSOperationWithView:(NSView *)aView insideRect:(NSRect)rect toData:(NSMutableData *)data;
+ (NSPrintOperation *)EPSOperationWithView:(NSView *)aView insideRect:(NSRect)rect toData:(NSMutableData *)data printInfo:(NSPrintInfo *)aPrintInfo;
+ (NSPrintOperation *)EPSOperationWithView:(NSView *)aView insideRect:(NSRect)rect toPath:(NSString *)path printInfo:(NSPrintInfo *)aPrintInfo;

+ (NSPrintOperation *)PDFOperationWithView:(NSView *)aView insideRect:(NSRect)rect toData:(NSMutableData *)data;
+ (NSPrintOperation *)PDFOperationWithView:(NSView *)aView insideRect:(NSRect)rect toData:(NSMutableData *)data printInfo:(NSPrintInfo *)aPrintInfo;
+ (NSPrintOperation *)PDFOperationWithView:(NSView *)aView insideRect:(NSRect)rect toPath:(NSString *)path printInfo:(NSPrintInfo *)aPrintInfo;

/* Set/get the current print operation for this thread.  If this is nil, there is no current operation for the current thread.
*/
+ (NSPrintOperation *)currentOperation;
+ (void)setCurrentOperation:(NSPrintOperation *)operation;

/* Returns whether the operation is for "copying" or "printing"
*/
- (BOOL)isCopyingOperation;

/* Do the print operation.  Returns YES if the operation completed successfully, NO if there was an error or the user cancelled the operation.  This can only be called once.  Create a new NSPrintOperation instance for each operation.  When this method completes, the object is removed from being the current operation if it is the current operation.
*/
- (BOOL)runOperation;

/* Do the print operation, with panels that are document-modal to a specific window.  didRunSelector should have the following signature:
- (void)printOperationDidRun:(NSPrintOperation *)printOperation success:(BOOL)success contextInfo:(void *)contextInfo;
*/
- (void)runOperationModalForWindow:(NSWindow *)docWindow delegate:(id)delegate didRunSelector:(SEL)didRunSelector contextInfo:(void *)contextInfo;

/* Allow developers to allow us to spawn a separate printing thread.
*/
- (void)setCanSpawnSeparateThread:(BOOL)canSpawnSeparateThread;
- (BOOL)canSpawnSeparateThread;

/* Control whether the print panel is shown during the operation (doesn't necessarily include alerts).
*/
- (void)setShowPanels:(BOOL)flag;
- (BOOL)showPanels;

/* Allow developers to supply their own Print panel if they like.
*/
- (void)setPrintPanel:(NSPrintPanel *)panel;
- (NSPrintPanel *)printPanel;

/* Allow developers to supply an accessory view
*/
- (void)setAccessoryView:(NSView *)aView;
- (NSView *)accessoryView;

#if MAC_OS_X_VERSION_10_2 <= MAC_OS_X_VERSION_MAX_ALLOWED

// Set or get a string that provides a hint about the type of print job.  This controls the set of items that appear in the Presets menu of the print panel presented by this operation, if it presents one.  The string must be one of the job style hint strings declared in NSPrintPanel.h, or nil to provide no hint.
- (void)setJobStyleHint:(NSString *)hint;
- (NSString *)jobStyleHint;

#endif

/* Set/get the current PrintInfo object for the operation.  Setting the PrintInfo object should be done very carefully.  The safest approach is to copy all the settings from the new object into current object, rather than replacing the object.
*/
- (void)setPrintInfo:(NSPrintInfo *)aPrintInfo;
- (NSPrintInfo *)printInfo;

/* The view being printed.
*/
- (NSView *)view;

/* The current page number.
*/
- (int)currentPage;

/* Set/get the page order that will be used to generate the pages in this job.  This is the physical page order of the pages.  It depends on the stacking order of the printer, the capability of the app to reverse page order, etc.
*/
- (void)setPageOrder:(NSPrintingPageOrder)order;
- (NSPrintingPageOrder)pageOrder;

/* The context for the output of this operation.
*/
- (NSGraphicsContext *)context;

/* Create the context using the current PrintInfo settings, stream, etc.  Should not be called directly.  This is called before any output is generated by the operation.
*/
- (NSGraphicsContext *)createContext;

/* Destroy the context.  This is called at the end of the operation.  Should not be called directly.
*/
- (void)destroyContext;

/* This spools the job, calls the Preview app, etc. -- whatever needs to be done to complete the operation.  Return YES is operation was completely successful, NO otherwise.  It is called after the job has been completely generated.  Should not be called directly.
*/
- (BOOL)deliverResult;

/* Clean up an operation after it has been completed.  The object removes itself from being the current operation if it is the current operation.  
*/
- (void)cleanUpOperation;


@end
