#import "ImagePicker.h"
#import <AssetsLibrary/AssetsLibrary.h>
#include "GameDataManager.h"
#include "utils.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)


#include "base/CCDirector.h"
#include "2d/CCLabel.h"
#import "platform/ios/CCEAGLView-ios.h"

ImagePickerBridge* __createImagePicker(const ImagePickerType type)
{
    return new ImagePickerIos(type);
}


ImagePickerIos::ImagePickerIos(const ImagePickerType type)
{
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
    
    UIImagePickerController* ipc = [[UIImagePickerController alloc] init];
    
    switch (type) {
        case CameraWithEdit:
            ipc.sourceType = UIImagePickerControllerSourceTypeCamera;
            ipc.allowsEditing = YES;
            break;
        case PhotoLibraryWithEdit:
            ipc.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
            ipc.allowsEditing = YES;
            break;
        case PhotoLibraryWithoutEdit:
            ipc.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
            ipc.allowsEditing = NO;
            break;
        case CameraWithoutEdit:
            ipc.sourceType = UIImagePickerControllerSourceTypeCamera;
            ipc.allowsEditing = NO;
            break;

        default:
            break;
    }
    
    

    [eaglview addSubview:ipc.view];
    
    delegate_ = [[ImagePickerIosDelegate alloc] init];
    delegate_.iosPicker = this;
    ipc.delegate = delegate_;
}

void ImagePickerIos::change_head_pic(const std::string& pic_name)
{
    ImagePickerBridge::change_head_pic(pic_name);
}

@interface ImagePickerIosDelegate ()

- (UIImage *)compressImage:(UIImage *)sourceImage toTargetWidth:(CGFloat)targetWidth;

@end

@implementation ImagePickerIosDelegate

+ (UIImage*)imageWithImageSimple:(UIImage*)image scaledToSize:(CGSize)newSize
{
    // Create a graphics image context
    UIGraphicsBeginImageContext(newSize);
    
    // Tell the old image to draw in this new context, with the desired
    // new size
    [image drawInRect:CGRectMake(0,0,newSize.width,newSize.height)];
    
    // Get the new image from the context
    UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
    
    // End the context
    UIGraphicsEndImageContext();
    
    // Return the new image.
    return newImage;
}

#pragma mark 保存图片到document
- (void)saveImage:(NSData *)imageData WithName:(NSString *)imageName
{
    //NSData* imageData = UIImageJPEGRepresentation(tempImage,1);
    NSString* user_path = [NSString stringWithFormat:@"%s", GDM->get_user_directory().c_str()];
    NSString* fullPathToFile = [NSString stringWithFormat:@"%@%@", user_path, imageName];
    [imageData writeToFile:fullPathToFile atomically:NO];
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    UIImage *image= [info objectForKey:@"UIImagePickerControllerEditedImage"];
    if (image == nil)
    {
        image= [info objectForKey:@"UIImagePickerControllerOriginalImage"];
    }
//    UIImage* theImage = [ImagePickerIosDelegate imageWithImageSimple:image scaledToSize:CGSizeMake(120.0, 120.0)];
    image = [self compressImage:image toTargetWidth:640];
    uint32_t time = tools::date();
    NSString* head_pic = [NSString stringWithFormat:@"%u_head_pic.jpg", time];
    head_pic = [NSString stringWithFormat:@"%u_%@", GDM->get_user_id(), head_pic];
    NSData *imageData = UIImageJPEGRepresentation(image,0.3);
    
    //image = [UIImage imageWithData:imageData];
    
    [self saveImage:imageData WithName:head_pic];
    std::string head_pic_name = std::string([head_pic UTF8String]);
    if (self.iosPicker) {
        self.iosPicker->change_head_pic(head_pic_name);
    }
    
    
    [picker.view removeFromSuperview];
    [picker release];
}
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    if (self.iosPicker) {
        self.iosPicker->do_cancel();
    }
    [picker.view removeFromSuperview];
    [picker release];
}


/*  压缩图片至目标尺寸
*
*  @param sourceImage 源图片
*  @param targetWidth 图片最终尺寸的宽
*
*  @return 返回按照源图片的宽、高比例压缩至目标宽、高的图片
*/
- (UIImage *)compressImage:(UIImage *)sourceImage toTargetWidth:(CGFloat)targetWidth {
    CGSize imageSize = sourceImage.size;
    
    CGFloat width = imageSize.width;
    CGFloat height = imageSize.height;
    
    CGFloat targetHeight = (targetWidth / width) * height;
    
    UIGraphicsBeginImageContext(CGSizeMake(targetWidth, targetHeight));
    [sourceImage drawInRect:CGRectMake(0, 0, targetWidth, targetHeight)];
    
    UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return newImage;
}
@end

#endif

