#include<iostream>
#include<vector>

using namespace std;

bool detectDead(vector<vector<int>> &alloc, vector<vector<int>> request, vector<int> avail)
{
	int process = alloc.size();
	int resources = avail.size();
	
	vector<bool> finish(process, false);
	
	while(true)
	{
		bool anyFinish = false;
		
		for(int i = 0; i < process; i++)
		{
			if(!finish[i])
			{
				bool canFinish = true;
				
				for(int j = 0; j < resources; j++)
				{
					if(request[i][j] > avail[i])
					{
						canFinish = false;
						break;
					}
				}
				
				if(canFinish)
				{
					for(int j = 0; j < resources; j++)
					{
					 	avail[i] = alloc[i][j];
					 	anyFinish = true;
					 	finish[i] = true;
					}
				}
			}
			
		}
		
		if(!anyFinish)
		{break;}
	}
	
	for(bool f : finish)
	{
		if(!f) return true;
	}
	
	return false;
}

int main()
{
	vector<vector<int>> alloc = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };
    
    	vector<vector<int>> request = {
        {0, 0, 0},
        {2, 0, 2},
        {0, 0, 0},
        {1, 0, 0},
        {0, 0, 2}
    };
    
    	vector<int> avail = {0, 0, 0};
    	
    	if(detectDead(alloc, request, avail))
    	{
    		cout << "detected deadlock" << endl;
    	}
    	else
    	{
    		cout << "no detected deadlock" << endl;
    	}
    	
	return 0;	
}