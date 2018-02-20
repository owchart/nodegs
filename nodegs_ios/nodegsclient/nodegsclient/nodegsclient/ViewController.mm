//
//  ViewController.m
//  node.gs
//
//  Created by GAIA-Develop on 2017/3/2.
//  Copyright © 2017年 GAIA-Develop. All rights reserved.
//

#import "ViewController.h"
#include "stdafx.h"
#include "DataCenter.h"
#include "LoginService.h"
#include "BaseService.h"
#include "HttpGetService.h"
#include "HttpPostService.h"
#include "Binary.h"
@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    int socket = BaseService::ConnectToServer(0, "app.17taohui.com", 8084, "", 0, "", "", "", 10);
    int a = 0;
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
