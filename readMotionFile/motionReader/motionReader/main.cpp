//
//  main.cpp
//  motionReader
//
//  Created by Huang, Yuchi on 11/20/16.
//  Copyright © 2016 Huang, Yuchi. All rights reserved.
//


#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <climits>
#define PI 3.14159265

using namespace std;
void printVecValue(vector<double> vec) {
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << endl;
    }
}

void printVecValueInt(vector<int> vec) {
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << endl;
    }
}

// compute distance of two players and the degree of 'looking to each other'
void line2TransMat(string line, vector<double>& trans, vector<int>& time) {
    vector<int> all_pos_space;
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == ' ') {
            all_pos_space.push_back(i);
        }
    }
    
    //cout << all_pos_space.size() << endl;
    
    for (int i = 0; i < all_pos_space.size(); ++i) {
        if (i == 0) {
            int hour = stod(line.substr(1, 2));
            time.push_back(hour);
            int mins = stod(line.substr(4, 2));
            time.push_back(mins);
            int secs = stod(line.substr(7, 2));
            time.push_back(secs);
            int rest = stod(line.substr(10, 6));
            time.push_back(rest);
            //printVecValueInt(time);
        }
        else {
            if (i == 1) {
                //	cout << "string : " << line.substr(all_pos_space[i - 1] + 1, all_pos_space[i] - all_pos_space[i - 1] - 1) << endl;
            }
            else {
                double value = stod(line.substr(all_pos_space[i - 1] + 1, all_pos_space[i] - all_pos_space[i - 1] - 1));
                trans.push_back(value);
            }
        }
    }
    double value = stod(line.substr(all_pos_space[all_pos_space.size() - 1] + 1, line.size() - 1 - all_pos_space[all_pos_space.size() - 1]));
    trans.push_back(value);
    //cout << trans.size() << endl;
    //printVecValue(trans);
    //cout << "double value : " << value << endl;
    //cout << " " << endl;
}

void distanceAndCosSim(vector<double> trans1, vector<double> trans2, double& dis, double& cosSim) {
    dis = sqrt((trans1[12] - trans2[12]) * (trans1[12] - trans2[12])
               + (trans1[13] - trans2[13]) * (trans1[13] - trans2[13])
               + (trans1[14] - trans2[14]) * (trans1[14] - trans2[14]));
    
    /*
     // transform (1,1,1) by the first transformation matrix
     double x1 = trans1[0] + trans1[4] + trans1[8] + trans1[12];
     double y1 = trans1[1] + trans1[5] + trans1[9] + trans1[13];
     double z1 = trans1[2] + trans1[6] + trans1[10] + trans1[14];
     
     // transform (1,1,1) by the second transformation matrix
     double x2 = trans2[0] + trans2[4] + trans2[8] + trans2[12];
     double y2 = trans2[1] + trans2[5] + trans2[9] + trans2[13];
     double z2 = trans2[2] + trans2[6] + trans2[10] + trans2[14];
     
     // vector 1: (x1, y1, z1) - (tx1, ty1, tz1)
     // minus vector 2: (tx2, ty2, tz2) - (x2, y2, z2)
     double v1x = x1 - trans1[12];
     double v1y = y1 - trans1[13];
     double v1z = z1 - trans1[14];
     
     double mv2x = trans2[12] - x2;
     double mv2y = trans2[13] - y2;
     double mv2z = trans2[14] - z2;
     */
    double v1x = trans1[0] + trans1[4] + trans1[8];
    double v1y = trans1[1] + trans1[5] + trans1[9];
    double v1z = trans1[2] + trans1[6] + trans1[10];
    
    double mv2x = -(trans2[0] + trans2[4] + trans2[8]);
    double mv2y = -(trans2[1] + trans2[5] + trans2[9]);
    double mv2z = -(trans2[2] + trans2[6] + trans2[10]);
    
    // measure the cos similarity
    double en1 = sqrt(v1x*v1x + v1y*v1y + v1z*v1z);
    double en2 = sqrt(mv2x*mv2x + mv2y*mv2y + mv2z*mv2z);
    cosSim = (v1x*mv2x + v1y*mv2y + v1z*mv2z) / en1 / en2;
}

int readMotionFile(string fileName, int n_player) {
    ifstream inFile(fileName.c_str());
    if (!inFile) {
        cerr << "File " << fileName << ".txt not found." << endl;
        return -1;
    }
    string line;
    int cur_total_player = 0;
    vector<vector<double>> cur_all_trans;
    cout << "cur_all_trans size is " << cur_all_trans.size() << endl;
    vector<double> temp1(n_player, numeric_limits<double>::max());
    vector<vector<double>> cur_all_dis(n_player,temp1);
    vector<double> temp2(n_player, -1);
    vector<vector<double>> cur_all_cosSim(n_player, temp2);
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        vector<double> cur_trans;
        vector<int> cur_time;
        line2TransMat(line, cur_trans, cur_time);
        cur_total_player++;
        if (cur_total_player < n_player) {
            cur_all_trans.push_back(cur_trans);
            cout << "cur_all_trans size is " << cur_all_trans.size() << endl;
        } else {
            cur_all_trans.push_back(cur_trans);
            cout << cur_all_trans.size() << endl;
            double avg_dis = 0;
            for (int i = 0; i < n_player; ++i) {
                for (int j = i + 1; j < n_player; ++j) {
                    double dis_ij, cosSim_ij;
                    cout << "begin to compute" << endl;
                    distanceAndCosSim(cur_all_trans[i], cur_all_trans[j], dis_ij, cosSim_ij);
                    cout << "distance between " << i + 1 << " and " << j + 1 << " are " << dis_ij << " and cosSim is " << cosSim_ij << endl;
                    cur_all_dis[i][j] = dis_ij;
                    cur_all_dis[j][i] = dis_ij;
                    avg_dis = avg_dis + dis_ij;
                    cur_all_cosSim[i][j] = cosSim_ij;
                    cur_all_cosSim[j][i] = cosSim_ij;
                }
            }
            avg_dis = 2 * avg_dis / (n_player*n_player - n_player);
            //cout << (n_player*n_player - n_player)/2 << endl;
            // compute nearest neighor
            vector<double> NNDs(n_player,0);
            double avg_NND = 0;
            for (int i = 0; i < n_player; ++i) {
                double min_cur = numeric_limits<double>::max();
                for (int j = 0; j < n_player; ++j) {
                    if (i != j && min_cur > cur_all_dis[i][j]) {
                        min_cur = cur_all_dis[i][j];
                    }
                }
                NNDs[i] = min_cur;
                avg_NND += min_cur;
            }
            cout << avg_dis << endl;
            avg_NND = avg_NND/n_player;
            cur_all_trans.clear();
            cur_total_player = 0 ;
        }
        //double theta = - asin (R13) * 180.0 / PI;
        //double psi = atan (R23/R33) * 180 / PI;
        //
        
        // Using istringstream to read the line into integers.
        //istringstream iss(line);
        //int sum = 0, next = 0;
        //while (iss >> next) sum += next;
        //outFile << sum << endl;
    }
    
    inFile.close();
    cout << "good" << endl;
    return 0;
}

int main() {
    string fileName = "/Users/yhuang001/Documents/projects/MulCAT/readMotionFile/readMotion/x64/Debug/test.txt";
    return readMotionFile(fileName, 2);
    
}
