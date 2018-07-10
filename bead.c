#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

void newParticipant(){
	FILE *f = fopen("input.txt", "a");
	char name[100];
	getchar();
	
	printf("Nev: ");
	fgets (name, 100, stdin);
	name[strlen(name) - 1] = 0;
	fprintf(f, "%s, ", name);
	
	printf("Email cim: ");
	fgets (name, 100, stdin);
	name[strlen(name) - 1] = 0;
	fprintf(f, "%s, ", name);
		
	printf("Telefonszam: ");
	fgets (name, 100, stdin);
	name[strlen(name) - 1] = 0;
	fprintf(f, "%s, ", name);
		
	printf("Utvonal(1- Parlament, 2- Hősök Tere, 3 - Vár): ");
	fgets (name, 100, stdin);
	name[strlen(name) - 1] = 0;
	fprintf(f, "%s, ", name);
		
	printf("Hany fo neveben jelentkezel: ");
	fgets (name, 100, stdin);
	name[strlen(name) - 1] = 0;
	fprintf(f, "%s, ", name);
		
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime ( &rawtime );
	fprintf ( f,"Datum: %s", asctime(timeinfo));
	fclose(f);
}

void modifyParticipant(){
	FILE *f1, *f2;
	char ch;
	int change, temp = 1;
	int boo = 0;
	
	f1 = fopen("input.txt", "r");
	fseek(f1, 0, SEEK_SET);
	
	printf("Melyik sort szeretne modositani?\n");
	scanf("%d", &change);
	f2 = fopen("seged.txt","w");
	while(ch != EOF)
	{
		ch = getc(f1);
		if(temp != change){
			putc(ch, f2);
		}if(temp==change && boo == 0){
			boo = 1;
			char name[100];
			getchar();
			
			printf("Nev: ");
			fgets (name, 100, stdin);
			name[strlen(name) - 1] = 0;
			fprintf(f2, "%s, ", name);
			
			printf("Email cim: ");
			fgets (name, 100, stdin);
			name[strlen(name) - 1] = 0;
			fprintf(f2, "%s, ", name);
			
			printf("Telefonszam: ");
			fgets (name, 100, stdin);
			name[strlen(name) - 1] = 0;
			fprintf(f2, "%s, ", name);
			
			printf("Utvonal(1- Parlament, 2- Hősök Tere, 3 - Vár): ");
			fgets (name, 100, stdin);
			name[strlen(name) - 1] = 0;
			fprintf(f2, "%s, ", name);
			
			printf("Hany fo neveben jelentkezel: ");
			fgets (name, 100, stdin);
			name[strlen(name) - 1] = 0;
			fprintf(f2, "%s, ", name);

			time_t rawtime;
			struct tm * timeinfo;
			time (&rawtime);
			timeinfo = localtime ( &rawtime );
			fprintf ( f2,"Datum: %s", asctime(timeinfo));
		}
		if(ch == '\n'){
			temp++;
		}
	}
	fseeko(f2,-1,SEEK_END);
	off_t position = ftello(f2);
	ftruncate(fileno(f2), position);
	
	fclose(f1);
	fclose(f2);
	remove("input.txt");
	rename("seged.txt","input.txt");
}

void deleteParticipant(int del){
	FILE *f1, *f2;
	char ch;
	int temp = 1;
	
	f1 = fopen("input.txt", "r");
	fseek(f1, 0, SEEK_SET);
	
	f2 = fopen("seged.txt","w");
	while(ch != EOF)
	{
		ch = getc(f1);
		if(temp != del){
			putc(ch, f2);
		}
		if(ch == '\n')
		{
			temp++;
		}
	}
	
	fseeko(f2,-1,SEEK_END);
	off_t position = ftello(f2);
	ftruncate(fileno(f2), position);
	
	fclose(f1);
	fclose(f2);
	remove("input.txt");
	rename("seged.txt","input.txt");
}

void endTour(int tourNumber){
	FILE *f;
    	f = fopen("input.txt", "w");
	fclose(f);
	/*char line[100];
	int lineCounter = 1;
	int value;
	
	while(fgets(line, sizeof(line), f)){
		strtok(line, ", ");
		strtok(NULL, ", ");
		strtok(NULL, ", ");
		value = atoi(strtok(NULL, ", "));
		
		if(value == tourNumber){
			//deleteParticipant(lineCounter);
			printf("%d", lineCounter);
		}else{
			lineCounter++;	
		}
	}*/
}

void startTour(int numb, int route){
	int randomNumber = rand() % 10;

	if(route == 1){
		printf("----TURA INDULHAT PARLAMENT VONALON----\n");
	}
	if(route == 2){
		printf("----TURA INDULHAT HOSOK TERE VONALON----\n");
	}
	if(route == 3){
		printf("----TURA INDULHAT VAR VONALON----\n", route);
	}
	sleep(1);
	int i;
	int parentToChild[2];
	int childToParent[2];
	if (pipe(parentToChild) < 0 || pipe(childToParent) < 0) {
		perror("Csovezetek-megnyitasi hiba!\n");
		exit(EXIT_FAILURE);
	}
	pid_t pid = fork();   //lemasolodik a childfolyamatba minden, ami felette van
	
	if (pid < 0){
		perror("Fork hiba!");
		exit(EXIT_FAILURE);
	}

	//child process
	if(pid == 0){
		close(parentToChild[1]);
		close(childToParent[0]);
		
		printf("----%d szamu SBD munkaba allt!\n", getpid());
		sleep(1);
		kill(getppid(), SIGUSR1); //sor vetelezese utan jel kuldes
		int price;
		read(parentToChild[0], &price, sizeof(int));
		printf("Reszveteli dij: %d \n", price);
		int ro;
		read(parentToChild[0], &ro, sizeof(int));
		printf("Utvonal: %d \n", ro);
		
		int beers = 5 * numb;
		write(childToParent[1], &beers, sizeof(int));
		int total = 1000 * numb;
		write(childToParent[1], &total, sizeof(int));
		close(childToParent[1]);
		printf("Sor és penz visszakuldve %d SDB\n", getpid());
		sleep(1);
		printf("Tura elindul\n");
		sleep(1);
		
		for(i = 0; i < randomNumber; i++) {
			printf("*** ...\n");
			sleep(1);
		}
		printf("*** Tura vege\n");
		sleep(1);
		kill(getppid(), SIGUSR2); //tura vege
		raise(SIGTERM);

	}else{
		close(parentToChild[0]);
		close(childToParent[1]);
		pause();
		int price = 1000;
		write(parentToChild[1], &price, sizeof(int));
		write(parentToChild[1], &route, sizeof(int));
		close(parentToChild[1]);

		printf("----Turaadatok elkuldve %d szamu SBD-nek\n-----", pid);
		int beers;
		read(childToParent[0], &beers, sizeof(int));
		int total;
		read(childToParent[0], &total, sizeof(int));
		close(childToParent[0]);
		printf("*** Adatok megerkeztek %d szymu SBD-tol:\n", pid);
		printf("*** Vetelezett sor mennyisege: %d liter\n", beers);
		printf("*** Beszedett dij osszege: %d \n", total);
		sleep(1);
		pause();
		endTour(route);
	}

}



void listParticipant(){
	int f=open("input.txt",O_RDONLY);
	char c;
	while (read(f,&c,sizeof(c))){ //use write for writing
		printf("%c",c); //we prints out the content of the file on the screen
	}
}

void countTourParticipant(){
	FILE *f;
    f = fopen("input.txt", "r");
	char line[100];

	int value;
	int numberOfParticipant;
	int tourOne = 0;
	int tourTwo = 0;
	int tourThree = 0;
	
	
	while(fgets(line, sizeof(line), f)){
		strtok(line, ",");
		strtok(NULL, ",");
		strtok(NULL, ",");
		value = atoi(strtok(NULL, ","));
		numberOfParticipant = atoi(strtok(NULL, ","));
		
		if(value == 1){
			tourOne += numberOfParticipant;
			if(tourOne > 10){
				startTour(tourOne, 1);
			}
		}else if(value == 2){
			tourTwo += numberOfParticipant;
			if(tourTwo > 10){
				startTour(tourTwo, 2);
			}
		}else if(value == 3){
			tourThree += numberOfParticipant;
			if(tourThree > 10){
				startTour(tourThree, 3);
			}
		}
	}
	//printf("%d, %d, %d", tourOne, tourTwo, tourThree);
	
	fclose(f);
}


void handler(int sigNumber) { }

int main()
{
	
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	
	int menu = -1;
	while(menu != 0){
		printf("\n[1] Uj jelentkezo felvetele\n");
		printf("[2] Jelentkezo modositasa\n");
		printf("[3] Jelentkezo torlese\n");
		printf("[4] Jelentkezok listazasa\n");
		//printf("[5] Proba\n");
		printf("[0] Kilepes\n");
		printf("Irja be a megfelelo szamot\n");
		
		scanf("%d", &menu);
		if(menu == 0){
			exit(0);
		}else if(menu == 1){
			newParticipant();
		}
		else if(menu == 2){
			modifyParticipant();
		}
		else if(menu == 3){
			int delete;
			printf("Melyik sort szeretne torolni?\n");
			scanf("%d", &delete);
			deleteParticipant(delete);
		}
		else if(menu == 4){
			listParticipant();
		}/*else if(menu = 5){
			countTourParticipant();
		}*/
		countTourParticipant(); //minden muvelet utan megnezzuk indulhat e tura
	}
}
