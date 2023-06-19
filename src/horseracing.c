#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_HORSES 3
#define TRACK_LENGTH 20



typedef struct {

    char name[50];

    int position;

} Horse;



void displayTrack(Horse horses[], int numHorses, int trackLength) {

  system("clear");

  for (int i = 0; i < numHorses; i++) {

    for (int j = 0; j < trackLength; j++) {

      if (j == horses[i].position) {

        printf("H");

      } else {

        printf("-");

      }

    }

    printf(" %s\n", horses[i].name);

  }

}



int main() {

  Horse horses[NUM_HORSES] = {

          {"Horse1", 0},

          {"Horse2", 0},

          {"Horse3", 0}

  };



  while (1) {

    displayTrack(horses, NUM_HORSES, TRACK_LENGTH);



    for (int i = 0; i < NUM_HORSES; i++) {

      horses[i].position += rand() % 3; // Randomly move 0, 1, or 2 spaces



      if (horses[i].position >= TRACK_LENGTH-1) {

        printf("%s wins!\n", horses[i].name);

        return 0;

      }

    }



    usleep(500000); // Pause for half a second

  }



  return 0;

} 