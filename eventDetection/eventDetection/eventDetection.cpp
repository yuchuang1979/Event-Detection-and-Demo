//
//  main.cpp
//  eventDetection
//
//  Created by Huang, Yuchi on 02/03/17.
//  Copyright © 2016 Huang, Yuchi. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>  
#include <fstream>
#include <iostream>
#include <functional>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <vector>
#include <math.h>
#include <climits>
#include <iomanip>
#define PI 3.14159265

using namespace std;


// get a numeric vector from a line
void stringline2Vector(string line, vector<double>& vec) {
	vector<int> all_pos_space;
	for (int i = 0; i < line.size(); ++i) {
		if (line[i] == ' ') {
			all_pos_space.push_back(i);
		}
	}
	vec.push_back(stod(line.substr(0, all_pos_space[0])));
	for (int i = 0; i < all_pos_space.size() - 1; ++i) {
		if (all_pos_space[i + 1] - all_pos_space[i] > 1) {
			vec.push_back(stod(line.substr(all_pos_space[i] + 1, all_pos_space[i + 1] - all_pos_space[i] - 1)));
		}
	}
	/*for (int i = 0; i < vec.size(); ++i) {
		cout << vec[i] << " ";
	}
	cout << "" << endl;
	*/
}


// event01 and event02 detection: one player leaves others and one player was left by others
int event0102Detection(vector<double> avg_NND, vector<vector<double>> avg_POS, vector<vector<double>> avg_prev_POS, 
	vector<int> prev_event, vector<int> pre_event_id, vector<int>& cur_event, vector<int>& cur_event_id, 
	double leave_NND_th, double move_POS_th) {
	//cur_event was initialized outside as 0s; cur_event_id was initialized outside as -1s
	int cnt_leave = 0; // if cnt_leave == 1, event 01 or 02 could happen. 
	int leave_id = -1; // used to record the id of leaving (left) player
	for (int i = 0; i < avg_NND.size() - 2; ++i) {
		if (i < 4) cout << avg_NND[i] << endl;
		if (avg_NND[i] == -1 || (avg_prev_POS[i][0] == -1 || avg_prev_POS[i][1] == -1)) {
			cout << "this player is invalid or in last interval it is invalid" << endl;
			continue; // if this player is invalid or just entered (not participating the game in the last interval), skip: because it has no history.
		}
		if (avg_NND[i] > leave_NND_th) {
			cnt_leave++;
			leave_id = i; // because i starts from 0, later i should +1
			if (cnt_leave > 1) {
				cout << "two players exceed the limits" << endl;
				cur_event[0] = 0;
				cur_event[1] = 0;
				return -1; // only one is left behind, or leaving.
			}
		}
	}
	if (cnt_leave == 0) {
		cur_event[0] = 0;
		cur_event[1] = 0;
		return -1; // no one is leaving or left behind
	}
	// cnt_leave == 1: now decide event01 or event02
	double POS_diff = sqrt((avg_POS[leave_id][0] - avg_prev_POS[leave_id][0]) * (avg_POS[leave_id][0] - avg_prev_POS[leave_id][0]) +
		(avg_POS[leave_id][1] - avg_prev_POS[leave_id][1]) * (avg_POS[leave_id][1] - avg_prev_POS[leave_id][1]));

	double POS_prev_avg_x = 0;
	double POS_prev_avg_y = 0;
	double POS_avg_x = 0;
	double POS_avg_y = 0;

	int group_cnt = 0;
	for (int i = 0; i < avg_NND.size() - 2; ++i) {
		if (i == leave_id) continue;
		if (avg_prev_POS[i][0] == -1 || avg_POS[i][0] == -1) continue;
		POS_prev_avg_x += avg_prev_POS[i][0];
		POS_prev_avg_y += avg_prev_POS[i][1];
		POS_avg_x += avg_POS[i][0];
		POS_avg_y += avg_POS[i][1];
		group_cnt++;
	}
	POS_prev_avg_x = POS_prev_avg_x / group_cnt;
	POS_prev_avg_y = POS_prev_avg_y / group_cnt;
	POS_avg_x = POS_avg_x / group_cnt;
	POS_avg_y = POS_avg_y / group_cnt;
	double POS_diff_group = sqrt((POS_prev_avg_x- POS_avg_x)*(POS_prev_avg_x - POS_avg_x) + (POS_prev_avg_y - POS_avg_y)*(POS_prev_avg_y - POS_avg_y));
	cout << "POS diff is " << POS_diff << " and "<< "POS_diff_group is " << POS_diff_group << endl;

	if ((POS_diff > move_POS_th && POS_diff_group <= move_POS_th/2 && prev_event[1] == 0)|| prev_event[0] == 1) { // move fast, or previous event 1  is 1
		cout << "Event 1 happening" << endl;
		cur_event[0] = 1;
		cur_event_id[0] = leave_id + 1;
	}
	if ((POS_diff <= move_POS_th/2 && POS_diff_group > move_POS_th && prev_event[0] == 0)|| prev_event[1] == 1) { // keep staying, or previous event 2  is 1
		cout << "Event 2 happening" << endl;
		cur_event[1] = 1;
		cur_event_id[1] = leave_id + 1;
	}
	return 1;
}

// event03 and event04 detection: concentrate or spread
void event0304Detection(double avg_dist, vector<int>& cur_event, double concentrate_th, double spread_th) {
	if (avg_dist == -1) {
		cur_event[2] = 0;
		cur_event[3] = 0;
	}
	if (avg_dist < concentrate_th) {
		cur_event[2] = 1;
		cout << "Event 3 happening" << endl;
	} else {
		cur_event[2] = 0;
	}
	if (avg_dist > spread_th) {
		cur_event[3] = 1;
		cout << "Event 4 happening" << endl;
	} else {
		cur_event[3] = 0;
	}
}


int readSavedFiles(string fileName_POS, string fileName_NND, int n_player, vector<double>& time_all, 
	vector<vector<vector<double>>>& POS_all, vector<vector<double>>& NND_all) {

	//read and process saved files and create corresponding vectors/matrices. process include:
	//    0. extract positions of each player from each line （each time interval） of out_xy_interval
	//    1. extract NNDs and group level distance measures of each player from each line （each time interval） of out_proximity_interval
	//

	// reading POS file
	ifstream inFile_POS(fileName_POS.c_str());
	if (!inFile_POS) {
		cerr << "POS File: " << fileName_POS << " not found." << endl;
		return -1;
	}

	string line;
	vector<double> temp_POS(2, -1);
	vector<vector<double>> cur_POS(n_player, temp_POS); // record positions (x y) of all players for current interval

	while (getline(inFile_POS, line)) {
		if (line.empty()) continue;
		vector<double> vec;
		stringline2Vector(line, vec);
		//cin.get();
		if (vec.size() == n_player * 2 + 2) {
			time_all.push_back(vec[0]);
			for (int i = 0; i < n_player; ++i) {
				cur_POS[i][0] = vec[(i + 1) * 2];
				cur_POS[i][0] = vec[(i + 1) * 2 + 1];
			}
			POS_all.push_back(cur_POS);
		}
	}
	inFile_POS.close();

	// reading NND file
	ifstream inFile_NND(fileName_NND.c_str());
	if (!inFile_NND) {
		cerr << "NND File: " << fileName_NND << " not found." << endl;
		return -1;
	}

	vector<vector<double>> cur_NND(n_player + 2);
	while (getline(inFile_NND, line)) {
		if (line.empty()) continue;
		vector<double> vec;
		stringline2Vector(line, vec);
		if (vec.size() == (n_player + 2) * 2 + 2) { // we have n_player pairs of (NND, NND_std) and (avg_NND, avg_NND_std), and (avg_dist, avg_dist_std).
			for (int i = 0; i < n_player + 2; ++i) 
				cur_NND(i) = vec[(i + 1) * 2];
			NND_all.push_back(cur_NND);
		}
	}
	inFile_NND.close();
	return 1;
}

void saveEventTime(int which_event, double cur_time, double pre_time, double time_length_th,
	int prev_event, int cur_event, int cur_event_id, vector<vector<double>>& event_starts, vector<vector<double>>& event_ends) {
	if (prev_event == 0 && cur_event == 1) {
		event_starts[which_event - 1].push_back(cur_time);
		event_ids[which_event - 1].push_back(cur_event_id);
	}

	if (prev_event == 1 && cur_event == 0) {
		double cur_event_legnth = pre_time - event_starts[which_event - 1][event_starts[which_event - 1].size() - 1];
		if (cur_event_legnth >= time_length_th) {
			event_starts[which_event - 1].push_back(pre_time);
		} else {
			event_starts[which_event - 1].pop_back();
			event_ids[which_event - 1].pop_back();
		}
	}
}

void saveEventTime(int which_event, double cur_time, double pre_time, double time_length_th,
	int prev_event, int cur_event, vector<vector<double>>& event_starts, vector<vector<double>>& event_ends) {
	if (prev_event == 0 && cur_event == 1) {
		event_starts[which_event - 1].push_back(cur_time);
	}

	if (prev_event == 1 && cur_event == 0) {
		double cur_event_legnth = pre_time - event_starts[which_event - 1][event_starts[which_event - 1].size() - 1];
		if (cur_event_legnth >= time_length_th) {
			event_starts[which_event - 1].push_back(pre_time);
		} else {
			event_starts[which_event - 1].pop_back();
		}
	}
}

int processDistEvents (vector<double> time_all, vector<vector<vector<double>>> POS_all, 
	vector<vector<double>> NND_all, vector<vector<double>>& event_starts, vector<vector<double>>& event_ends, vector<vector<int>>& event_ids,
	double leave_NND_th, double move_POS_th, double concentrate_th, double spread_th, double time_length_th) {
	if (POS_all.size() != NND.all.size()) {
		cerr << "Cannot perform event detection because of incomplete saved files" << endl;
		return -1;
	}

	// data preparation
	vector<int> prev_event(4, 0);
	vector<int> pre_event_id(2, -1); // only for event 01 and 02
	vector<int> cur_event(4, 0);
	vector<int> cur_event_id(2, -1); // only for event 01 and 02
	// perform distance based event detection. 
	for (int i = 1; i < POS_all.size(); ++i) { // starting from 1, not 0: time interval 1 has a previous interval -- 0 		
		// data preparation
		vector<double> avg_NND = NND_all[i];
		vector<double> avg_POS = POS_all[i];
		vector<double> avg_prev_POS = POS_all[i - 1];
		event0102Detection(avg_NND, avg_POS, avg_prev_POS, prev_event, pre_event_id, cur_event, cur_event_id, leave_NND_th, move_POS_th);
		saveEventTime(1, time_all[i], time_all[i - 1], time_length_th, prev_event[0], cur_event[0], cur_event_id[0], event_starts, event_ends);
		saveEventTime(2, time_all[i], time_all[i - 1], time_length_th, prev_event[1], cur_event[1], cur_event_id[1], event_starts, event_ends);

		event0304Detection(avg_NND(avg_NND.size() - 1), cur_event, concentrate_th, spread_th);
		saveEventTime(3, time_all[i], time_all[i - 1], time_length_th, prev_event[2], cur_event[2], event_starts, event_ends);
		saveEventTime(4, time_all[i], time_all[i - 1], time_length_th, prev_event[3], cur_event[3], event_starts, event_ends);
		
	
	}

}

	/*
	





	double cur_time_upperbound = start_time + time_slot;
	while (getline(inFile, line)) {
		if (line.empty()) continue;
		vector<double> cur_trans;
		double cur_time;
		bool valid_time = false;
		//a valid line means this transformation matrix is valid for one player at this time (motion capture has already begun to track this player)
		//a valid time is used to label whether the current time stamp is in the vaild range of processing: [start_time, end_time]
		bool valid_line = line2TransMat(line, cur_trans, cur_time, start_time, end_time, valid_time);
		if (!valid_time) continue;
		// count the valid lines
		cur_all_trans.push_back(cur_trans);
		if (valid_line) {
			cur_all_POS[cur_total_player][0] = cur_trans[12]; //x axis
			cur_all_POS[cur_total_player][1] = cur_trans[14]; //y axis
			cur_valid++;
		}
		cur_total_player++; // reading for current frame from the log file ends. Now increase cur_total_player by 1 
		// for write to disk: (POS info of each frame)
		if (cur_total_player == 1) {
			char buffer_time[100];
			int temp_time = sprintf(buffer_time, "%6f", cur_time);
			outfile_xyz << buffer_time << " ";
		}
		
		//cur_total_player ranging from 0 to n_player. When cur_total_player == n_player it will be reset to 0, after the processing of current time stamp
		outfile_xyz << setw(8) << cur_trans[12] << " ";
		outfile_xyz << setw(8) << cur_trans[13] << " ";
		outfile_xyz << setw(8) << cur_trans[14] << " ";

		// when cur_total_player < n_player, store all trans matrice in a 2D matrice
		if (cur_total_player == n_player) {
			// when info of all players for current time stamp has been collected for the log file
			outfile_xyz << endl; // when cur_total_player == n_player, the writing for the current row of POS ends.
			// if there is only one or 0 valid player, you do not have any NND to compute. 
			if (cur_valid < 2) {
				cur_all_trans.clear();
				cur_total_player = 0;
				cur_valid = 0;
				// do not have to write to disk ... jump to next frame (time stamp)
				continue;
			}

			//compute cur_all_dis and cur_look, avg_dis
			double avg_dis = 0;
			int avg_n = 0;
			vector<double> cur_look;
			vector<int> cur_look_id1, cur_look_id2;
			for (int i = 0; i < n_player; ++i) {
				for (int j = i + 1; j < n_player; ++j) {
					double dis_ij, cosSim1_ij, cosSim2_ij;
					distanceAndCosSim(cur_all_trans[i], cur_all_trans[j], dis_ij, cosSim1_ij, cosSim2_ij);
					//cout << "distance between " << i + 1 << " and " << j + 1 << " are " << dis_ij << "; cosSim1 is " << cosSim1_ij << "; cosSim2 is " << cosSim2_ij << endl;
					cur_all_dis[i][j] = dis_ij;
					cur_all_dis[j][i] = dis_ij;
					if (dis_ij != numeric_limits<double>::max()) {
						avg_dis = avg_dis + dis_ij;
						avg_n++;
					}
					double cosSim_ij = (cosSim1_ij + cosSim2_ij) / 2 * exp(-dis_ij/4);
					if (cosSim_ij > look_th) {
						cur_look.push_back(cosSim_ij);
						cur_look_id1.push_back(i);
						cur_look_id2.push_back(j);
					}
				}
			}
			avg_dis = avg_dis / avg_n;

			// compute nearest neighor and avg_NND of each stamp
			vector<double> NNDs(n_player, numeric_limits<double>::max());
			vector<double> NND_ids(n_player, -1);
			double avg_NND = 0;
			double avg_NND_n = 0;
			for (int i = 0; i < n_player; ++i) {
				double min_cur = numeric_limits<double>::max();
				int id_cur = -1;
				for (int j = 0; j < n_player; ++j) {
					if (i != j && min_cur > cur_all_dis[i][j]) {
						min_cur = cur_all_dis[i][j];
						id_cur = j;
					}
				}
				NNDs[i] = min_cur;
				NND_ids[i] = id_cur;
				if (id_cur != -1) {
					avg_NND += min_cur;
					avg_NND_n++;
				}
			}

			int temp;
			char buffer[100];
			//cout << "\n" << "time : ";
			temp = sprintf(buffer, "%6f", cur_time);
			outfile_proximity << buffer;
			//cout << buffer << " : ";
			if (cur_look.size() > 0) {
				outfile_lookEachOther << buffer;
			}
			//write look to each other prob to file
			if (cur_look.size() > 0) {
				for (int ind_look = 0; ind_look < cur_look.size(); ++ind_look) {
					temp = sprintf(buffer, " %6f ", cur_look[ind_look]);
					outfile_lookEachOther << buffer;
					outfile_lookEachOther << cur_look_id1[ind_look] + 1;
					outfile_lookEachOther << " ";
					outfile_lookEachOther << cur_look_id2[ind_look] + 1;
				}
			}
			if (cur_look.size() > 0) outfile_lookEachOther << "\n";
			//end to write look to each other prob to file

			//print to screen the avg distance, current nnd values of each player; then write to disk by outfile_proximity
			//cout << "\n" << "average distance is " << avg_dis << endl;
			for (int i = 0; i < n_player; ++i) {
				//cout << "the NND of the player " << i + 1 << " is " << NNDs[i] << " with the player " << NND_ids[i] + 1 << endl;
			}
			avg_NND = avg_NND / avg_NND_n;
			//cout << "the average NND is " << avg_NND << endl;
			// reset some loop value
			cur_all_trans.clear();
			cur_total_player = 0;
			cur_valid = 0;

			for (int i = 0; i < NNDs.size(); ++i) {
				if (NNDs[i] != numeric_limits<double>::max()) {
					temp = sprintf(buffer, " %8f ", NNDs[i]);
				}
				else {
					temp = sprintf(buffer, " %8d ", -1);
				}
				outfile_proximity << buffer;
				outfile_proximity << NND_ids[i] + 1;
			}
			temp = sprintf(buffer, " %6f %6f\n", avg_NND, avg_dis);
			outfile_proximity << buffer;

			// Use info NND_interval to compute mean and std for that interval; do event detection
			if (cur_time >= cur_time_upperbound) {
				cout << "current time is: " << cur_time << endl;
				temp = sprintf(buffer, "%4f %4f ", cur_time_upperbound - time_slot, cur_time_upperbound);
				outfile_proximity_interval << buffer;
				outfile_xy_interval << buffer;
				//outfile_events << buffer;
				// compute interval means and stds of NNDs, avg_NND and avg_dis
				for (int i = 0; i < NND_interval.size(); ++i) {
					double mean, stdev;
					if (NND_interval[i].size() == 0) {
						mean = -1;
						stdev = 0.0;
						
					}
					else if (NND_interval[i].size() == 1) {
						mean = NND_interval[i][0];
						stdev = 0;
					}
					else {
						meanSTD(NND_interval[i], mean, stdev);	
						if (stdev < 0.00001) {
							stdev = 0;
						}
					}
					// write to disk
					//avg_NND_vec[i] = mean; //average NND for each player, in an interval
					outfile_proximity_interval << setw(12) << mean << " ";
					outfile_proximity_interval << setw(12) << stdev << " ";
				}
				// write to disk
				outfile_proximity_interval << endl;

				// event detection
				// compute avg_POS
				for (int i = 0; i < n_player; ++i) {
					if (POS_cnt_interval[i] > 0) {
						avg_POS[i][0] = POS_sum_interval[i][0] / POS_cnt_interval[i];
						avg_POS[i][1] = POS_sum_interval[i][1] / POS_cnt_interval[i];
					} else {
						avg_POS[i][0] = -1;
						avg_POS[i][1] = -1;
					}
					outfile_xy_interval << setw(12) << avg_POS[i][0] << " ";
					outfile_xy_interval << setw(12) << avg_POS[i][1] << " ";
				}
				outfile_xy_interval << endl;
				// 
				//a.one player leaves others: if its nnd >leave_NND_th && others < leave_NND_th && its (|POS_diff| > move_POS_th || prev_event_1 ==1) && |others_diff| < move_POS_th
			    //b.one player was left by others if its nnd >leave_NND_th && others < leave_NND_th && its (|POS_diff| < move_POS_th || prev_event_2 ==1) && |others_diff| > move_POS_th
				//c.group concentrate avg_nnd < concentrate_th
				//d.group spread out  avg_nnd > concentrate_th
				//int temp_status = event0102Detection(avg_NND_vec, avg_POS, avg_prev_POS, prev_event, prev_event_id, cur_event, cur_event_id, leave_NND_th, move_POS_th);
				//event0304Detection(avg_NND_vec[avg_NND_vec.size() - 1], cur_event, concentrate_th, spread_th);
				// event detection ends. Parameters updating:
				//avg_prev_POS = avg_POS;
				//prev_event = cur_event;
				//prev_event_id = cur_event_id; // only for event 1 and event 2

				// clear POS_sum_interval and POS_cnt_interval
				for (int i = 0; i < n_player; ++i) {
					POS_sum_interval[i][0] = 0;
					POS_sum_interval[i][1] = 0;
					POS_cnt_interval[i] = 0;
				}

				// update cur_time_upperbound
				cur_time_upperbound = cur_time_upperbound + time_slot;
				// clear each dim of NND_interval
				for (int i = 0; i < NND_interval.size(); ++i) {
					NND_interval[i].clear();
				}
			}

			// just push NNDs, avg_NND and avg_dis to NND_interval
			for (int i = 0; i < NNDs.size(); ++i) {
				if (NNDs[i] != numeric_limits<double>::max()) {
					NND_interval[i].push_back(NNDs[i]);
				}
			}
			NND_interval[NNDs.size()].push_back(avg_NND);
			NND_interval[NNDs.size() + 1].push_back(avg_dis);
			
			// just add POS into POS_interval_sum
			for (int i = 0; i < n_player; ++i) {
				if (cur_all_POS[i][0] != -1 && cur_all_POS[i][1] != -1) {
					POS_sum_interval[i][0] += cur_all_POS[i][0];
					POS_sum_interval[i][1] += cur_all_POS[i][1];
					POS_cnt_interval[i]++;
				}
			}
			//prev_all_POS = cur_all_POS;
		}

	}

	inFile.close();
	outfile_proximity.close();
	outfile_proximity_interval.close();
	outfile_lookEachOther.close();
	outfile_xyz.close();
	outfile_xy_interval.close();
	//outfile_events.close();
	return 0;
}*/

int main(int argc, char* argv[]) {
	double threshold_look;
	if (argc == 2) {
		threshold_look = atof(argv[1]);
		//cout << threshold << endl;
	}
	else {
		threshold_look = 0.6;
	}

	double leave_NND_th = 4;  
	double move_POS_th = 0.8;
	double concentrate_th = 1.0;
	double spread_th = 3.0;

	//string fileName = "C:/Yuchi/20161105_124309.7z/20161105_124309/player_log_20161105_111044(scenario1).log";
	//string fileName = "C:/Yuchi/20161105_125351.7z/20161105_125351/player_log_20161105_111044(scenario1).log";
	//string fileName = "C:/Yuchi/20161105_125351.7z/20161105_125351/player_log_20161105_111044(scenario1)_test.txt";
	string fileName = "C:/Yuchi/20161105_131750.7z/20161105_131750/player_log_20161105_131641(scenario2).log";
	//string fileName = "C:/Yuchi/20161105_132930.7z/20161105_132930/player_log_20161105_131641(scenario2).log";
	//string fileName = "C:/Yuchi/20161105_132930.7z/20161105_132930/player_log_20161105_131641(scenario2)_test.txt";

	//double start_time = (double)(12 * 3600 + 43 * 60 + 10) + 0.00;
	//double start_time = (double)(12 * 3600 + 54 * 60 + 00) + 0.29;
	double start_time = (double)(13 * 3600 + 17 * 60 + 59) + 0.30;
    //double start_time = (double)(13 * 3600 + 29 * 60 + 39) + 0.32;

	//double end_time = (double)(12 * 3600 + 52 * 60 + 58) + 0.90;
	//double end_time = (double)(13 * 3600 + 06 * 60 + 55) + 0.50;
	double end_time = (double)(13 * 3600 + 26 * 60 + 41) + 0.57;
	//double end_time = (double)(13 * 3600 + 42 * 60 + 27) + 0.69;

	string fileName_POS = "out_xy_interval.txt";
	string fileName_NND = "out_proximity_interval.txt";
	vector<vector<vector<double>>> POS_all;
	vector<vector<double>> NND_all;
	vector<double> time_all;
	readSavedFiles(fileName_POS, fileName_NND, 6, time_all, POS_all, NND_all);
}
