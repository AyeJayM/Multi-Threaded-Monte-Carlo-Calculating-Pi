#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <utility>
#include <vector>
#include <thread>
#include <math.h>

using namespace std;

long long value;
unsigned long long minimum;
unsigned long long maximum;

int desiredThreads;
int desiredPoints;

float pointsInCircle;
float pointsEvaluated;

mutex myMutex;

void sumUp(float& pointsInCircle, float& pointsEvaluated, 
   unsigned long long beg, unsigned long long end)
   {

    lock_guard<mutex> myLock(myMutex);

    for (int it= beg; it <= end; ++it)
    {

       
    float xCoordinate = ( (double) rand() ) / ( (double)RAND_MAX);
    float YCoordinate = ( (double) rand() ) / ( (double)RAND_MAX);
    pointsEvaluated++;

    if ( (pow(xCoordinate, 2.0) + pow(YCoordinate, 2.0)) <= 1.0)
        {
            pointsInCircle++;
        }

   }

   }

int main(int argc, char** argv)
{

  srand(time(nullptr));

  cout << endl;

  pointsEvaluated = 0;
  pointsInCircle = 0;

  //BEGIN INPUT TESTING

  if ( (argc >= 4) || (argc <= 2) )
  {
    cout << "INVALID INPUT" << endl;
    cout << "usage: monte [1...10] [10...1000000]\n\n";

    return 0;
  }
  else
  {
  
  desiredThreads = atoi(argv[1]);
  desiredPoints = atoi(argv[2]);

    // Threads must be from 1 to 10.
    if( (desiredThreads < 1 || desiredThreads > 10) )
    {
        cout << "INVALID INPUT" << endl;
        cout << "usage: monte [1...10] [10...1000000]\n\n";

        return 0;
    }
    // Points must be from 10 to 1,000,000 
    else if( (desiredPoints < 10 || desiredPoints > 1000000 ) )
    {
        cout << "INVALID INPUT" << endl;
        cout << "usage: monte [1...10] [10...1000000]\n\n";

        return 0;
    }
  }

  //END INPUT TESTING

  // ASSIGN USER DESIRED # OF POINTS TO "VALUE" VARIABLE
  value = desiredPoints;


  auto start = chrono::system_clock::now();

  // ASSIGN USER DESIRED THREAD AMOUNT TO THREAD VARIABLE
  int threads = desiredThreads;

  thread t[threads];
  long long slice = value / threads;
  int startIdx=0;

  for (int i = 0; i < threads; ++i) 
  {
    cout << "Thread[" << i << "] - slice ["
         << startIdx << ":" << startIdx+slice-1 << "]" << endl;
    t[i] = thread(sumUp, ref(pointsInCircle), ref(pointsEvaluated), startIdx, startIdx+slice-1);
    startIdx += slice;
  }



  for (int i = 0; i < threads; ++i)
     t[i].join();

  chrono::duration<double> dur= chrono::system_clock::now() - start;
  cout << "Execution Time: " << dur.count() << " seconds" << endl;
  cout << "\nTotal Points In Circle: " << pointsInCircle << endl;
  cout << "\nTotal Points Evaluated: " << pointsEvaluated << endl;

  float ratio = pointsInCircle/pointsEvaluated;
  float ourPi = 4.0 * ratio;

  printf("\nRatio = %f \nOur Estimated Pi = %f\n",
               ratio, ourPi);

  float piDelta = M_PI - ourPi;
  printf("\nPi Delta (Difference from true Pi) = %f\n", piDelta);

  cout << endl;

}