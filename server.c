#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 512
typedef enum {false, true} bool;




int main(int argc , char *argv[]){
	
	int server, ret;
	 struct sockaddr_in local_addr;

	int newSocket;
	struct sockaddr_in remote_addr;

	socklen_t size;

	char userInput[MAX];
	pid_t childpid;

	server = socket(AF_INET, SOCK_STREAM, 0);
	if(server < 0){
		printf("Connection error\n");
		exit(1);
	}
	memset(&local_addr, '\0', sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(atoi(argv[1]));
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(server, (struct sockaddr*)&local_addr, sizeof(local_addr));
	if(ret < 0){
		printf("Binding Error.\n");
		exit(1);
	}
	printf("Command server listening on local host port %d\n",atoi(argv[1]));

	if(listen(server, 10) != 0){
		printf("Binding Error.\n");
	}


	while(1){
        // getting the first word of the input
        char *getFirstWord(char mainArr[] , int spacesArr[]){
                static char firstWord[MAX];
                for(int i=0 ; i<spacesArr[0] ; i++){
                    firstWord[i] = mainArr[i];
                } 
                return firstWord ; 
            }
            // finding spaces in the input 
        int *findSpaces(char arr[]){
            int j = 0 ;
            static int newArr[MAX];
            for(int i = 0 ; i<strlen(arr) ; i++ )
            {
            if(arr[i] == ' ')
                {
                newArr[j] = i;
                j = j+1;
                } 
            }
            return newArr ;
        };
        //check if the input is integer or not
        int checkInt(char arr[]){ 
            for(int i = 0 ; i<strlen(arr) ; i++){
                if(arr[i] >= 48 && arr[i] <= 57){
                    return 1; 
                };
                return 0 ;
            }   
            
        }
        // doing Calculations
        char *Calculate(char mainArr[] , int spacesArr[]){
            char *firstWord = getFirstWord(mainArr , spacesArr);
            char firstNumber[MAX];
            char secondNumber[MAX];
            bool notInt = false;
            int j = 0 ; 
            int l = 0 ;
            for (int i = 4 ; i<MAX ; i++){
                firstNumber[j] = mainArr[i];
                j++ ;  
            } ;
            for (int i = spacesArr[1]+1 ; i<MAX ; i++){
                secondNumber[l] = mainArr[i];
                l++;
            }

            if(checkInt(firstNumber) && checkInt(secondNumber)){
                static char resultArr[MAX];
                int result ;
                float divResult;
                if(strcmp(firstWord , "SUM") == 0){
                    result = atoi(firstNumber) + atoi(secondNumber);
                    sprintf(resultArr, "%d" , result);
                    return resultArr;
                }
                else if(strcmp(firstWord , "SUB") == 0){
                    result = atoi(firstNumber) - atoi(secondNumber);
                    sprintf(resultArr, "%d" , result);
                    return resultArr;
                }
                else if(strcmp(firstWord , "MUL") == 0){
                    result = atoi(firstNumber) * atoi(secondNumber);
                    sprintf(resultArr, "%d" , result);
                    return resultArr;
                }
                else if(strcmp(firstWord, "DIV") == 0){
                    divResult = atof(firstNumber) / atof(secondNumber);
                    sprintf(resultArr, "%.2f" , divResult);
                    return resultArr;
                }
            }    
            else {
                return "please enter 2 valid numbers.";
            }
        }



        // the echo function
        char *echo(char mainArr[] , int spacesArr[]){
            char *firstWord = getFirstWord(mainArr,spacesArr);
            static char sentence[MAX-4];
            int j = 0 ;
            for(int i=5 ; i < MAX ; i++){
            sentence[j] = mainArr[i];
            j++;
            }
            return sentence ;
        }



        // reversing the input in case RECHO
        char *recho(char arr[]){
            int j = strlen(arr)-1;
            static char newArr[MAX];
            for(int i = 0 ; i < MAX ; i++){
                newArr[i] = arr[j] ; 
                j--;
            }
            return newArr;
        }

            char *mainFun(char arr[]){
            
            static char newArr[512];
            // locating spaces
            int *spacesArr = findSpaces(arr);
            
            // sperating the first word
            char *firstWord = getFirstWord(arr , spacesArr);

            if(strcmp(firstWord,"SUM") == 0|| strcmp(firstWord,"SUB") == 0 || strcmp(firstWord,"MUL") == 0|| strcmp(firstWord,"DIV") == 0){
                return Calculate(arr , spacesArr);
            }
            else if(strcmp(firstWord , "ECHO") == 0){
                return echo(arr , spacesArr);
            }
            else if(strcmp(firstWord , "RECHO") == 0){
                return recho(echo(arr , spacesArr));
            }
            else if(strcmp(arr  ,"HELP") == 0){
                return "Command Server commands are\nECHO text : This is echo text message back\nRECHO text : This is echo text message in reverse order\nSUM A B : This is sum A + B and print result\nSUB A B : This is subtract B from A and print result\nMUL A B : This is multiply A and B and print result\nDIV A B : This is divide A and B and print result.";
            }
            else{
                sprintf(newArr ,"400 %s Command not implemented." , firstWord);
                return newArr;
            }

        }
        

		newSocket = accept(server, (struct sockaddr*)&remote_addr, &size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));

		if((childpid = fork()) == 0){
			close(server);
			while(1){
				recv(newSocket, userInput, 512, 0);
                printf("input : ");
                puts(userInput); 
                send(newSocket, mainFun(userInput), MAX-1 , 0);
                memset(&userInput , 0 , MAX);
                mainFun(userInput);
				
			}
		}

	}

	close(newSocket);
	return 0;
}
