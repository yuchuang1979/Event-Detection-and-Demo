//
//  main.cpp
//  vectorOfVector
//
//  Created by Huang, Yuchi on 12/3/16.
//  Copyright © 2016 Huang, Yuchi. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    vector<int> temp;
    vector<vector<int>> test(2, temp);
    test[0].push_back(1);
    test[0].push_back(2);
    test[1].push_back(3);
    test[1].push_back(4);
    test[1].push_back(5);
    
    cout<< test.size() << endl;
    cout<< test[0].size() << endl;
    cout<< test[1].size() << endl;
    //cout<< test[0][0] << " " << test[0][1] << endl;
    return 0;
}
