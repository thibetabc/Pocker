//
//  IosPay.h
//  pokerclient
//
//  Created by qqronaldz on 15/10/27.
//
//

#ifndef IosPay_h
#define IosPay_h

#import <UIKit/UIKit.h>

#import <StoreKit/StoreKit.h>

@interface PaymentViewController : UIViewController<SKPaymentTransactionObserver,SKProductsRequestDelegate>

@property (strong, nonatomic) IBOutlet UITextField *productID;

@property (strong, nonatomic) IBOutlet UIButton *purchase;

- (IBAction)purchaseFunc:(id)sender;
- (void)sendBuy : (NSString *)s_product;
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray<SKPaymentTransaction *> *)transactions;

@end
#endif
