#include <unistd.h>  
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <json/json.h>
#include <pthread.h>
#include "fstream"
#include "assert.h"
using namespace std;
//#define NUM_THREADS 100
//#define NUM_CROND 100

static int NUM_THREADS;
static int NUM_CROND;

Json::Reader reader;
Json::FastWriter fast;
Json::Value root,root_new;

int setcount = 0;

void update(Json::Value& a, Json::Value& b) {
	if (!a.isObject() || !b.isObject()) 
	{
		cout << "json value error"<<endl;
		return;
	}
	for (const auto& key : b.getMemberNames()) {
		if ( b[key].isObject() && a[key].isObject() ) {
			update(a[key], b[key]);
		} else {
			a[key]=b[key];
		}
	}
		cout << "json value success"<<endl;
} 

int ReadFromFile(Json::Value &root, string filename) {
	ifstream fin;
	fin.open(filename);
	assert(fin.is_open());
	//cout<< fin <<endl;
	//string aa = "{}";
	if(!reader.parse(fin, root, false))
	{
		cout<<"data is null"<<endl;
		return -1;
	}
	//cout<<fast.write(root) <<endl;	
	return 0;
}

void* on_work( void* args )
{
	cout<<fast.write(root)<<endl;
	cout<<fast.write(root_new)<<endl;
	for(int i=0; i<NUM_CROND; i++)
	{
		Json::Value root_tmp;
		root_tmp = root;	
		//执行更新json操作
		update(root_tmp, root_new);	
		setcount++;
		cout<<fast.write(root_tmp)<<endl;
	}
}

int main (int argc, char **argv)   
{  	
	//get 输入参数：参数1 创建线程数 参数2 线程循环次数
	if(argc != 3)
	{
		printf("param error");
		return -1;
	}
	NUM_THREADS = atoi(argv[1]);
	NUM_CROND = atoi(argv[2]);
	//printf("param 1:%d, param 2:%d",NUM_THREADS,NUM_CROND);
		
	ReadFromFile(root, "test.json");
	ReadFromFile(root_new, "test_new.json");
	pthread_t tids[NUM_THREADS];
	int count=0;  
	
	//get开始时间--微秒
	uint64_t begin,end;
	struct timespec ltv;
	(void)clock_gettime(CLOCK_REALTIME, &ltv);
	begin = (uint64_t)ltv.tv_sec * 1000 * 1000 + (uint64_t)ltv.tv_nsec / 1000;
	
	//创建多线程
	for(int i = 0; i<NUM_THREADS; ++i)
	{	
		int ret = pthread_create(&tids[i],NULL,on_work,NULL);
		if(ret != 0)
		{
			//cout<<"pthread_create error:error_code="<<ret<<endl;
		}
		else{
			
			//cout<<"pthread_create success pthread id="<<i<<endl;
		}
		ret = pthread_join(tids[i], NULL);
		if( ret != 0 )
		{
			//cout<<"pthread_join error:error_code="<<ret<<endl;
		}
	}

	//主线程等待子线程执行完毕
	while(setcount < NUM_THREADS * NUM_CROND){}
	
	//get结束时间
	(void)clock_gettime(CLOCK_REALTIME, &ltv);
	end = (uint64_t)ltv.tv_sec * 1000 * 1000 + (uint64_t)ltv.tv_nsec / 1000;
	//end = clock();
	//unsigned runtime = (end - begin)*1.0 / CLOCKS_PER_SEC * 1000;
	
	//输出时间结束线程
	unsigned runtime = end - begin;
	printf("runtime = %d\n", runtime);
	pthread_exit(NULL);
	return 0;  
} 
