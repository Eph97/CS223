#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>



enum state {SCANNING, TPRKT_START, 
	LAT_FOUND, LAT_READ_SINGLE, 
	LAT_READ_DOUBLE, 
	LAT_END, LON_FOUND, LON_READ_SINGLE,
	LON_READ_DOUBLE, 
	LON_END, ELE_FOUND, ELE_READING, 
	TIME_SEARCH, TIME_ELE_FOUND, TIME_SCAN, 
	TPRK_DONE, DOUBLE_TEXT, SINGLE_TEXT, LAT_CAND,
	LON_CAND};




bool checkMatch(char *MainString, char *subString);

void read_line(char s[], int max);


int main(int argc, char **argv)
{

	char a[10000];


	read_line(a, 9999);

	enum state curr = SCANNING;
	int pos = 0;

		while (a[pos]){
			switch (curr)
		{
				case SCANNING:

					//printf("%c", a[pos]);
					 if (a[pos] == '<'){
					 	
						
						if(checkMatch(&a[pos], "<trkpt") && ( isspace(a[pos+6]))){
							//printf("%s\n", &a[pos]);

							
							curr = TPRKT_START;
						}
					 }
					break;
				case TPRKT_START:
					if(a[pos] == '\"'){
						curr = DOUBLE_TEXT;
					}
					if(a[pos] == '\''){
						curr = SINGLE_TEXT;
					}
					else if(a[pos] =='l'){
						if(checkMatch(&a[pos], "lat")){
							curr = LAT_CAND;
							pos+= 2;
					}
						
					
						}
						break;

				case DOUBLE_TEXT:
					if(a[pos] == '\"'){
						curr = TPRKT_START;
					}
					break;

				case SINGLE_TEXT:
					if(a[pos] == '\''){
						curr = TPRKT_START;
					}
					break;

				case LAT_CAND:
			//	printf("%c", a[pos]);

					if (a[pos] == '='){
						curr = LAT_FOUND;
					}
					else if (!isspace(a[pos])){
						curr = TPRKT_START;
					}
					break;

				case LAT_FOUND:
						if(a[pos] == '\''){
				//			printf("LAT_READ");
							curr = LAT_READ_SINGLE;
						}
						else if(a[pos] == '\"'){
				//			printf("LAT_READ");
							curr = LAT_READ_DOUBLE;
						}
						break;
				case LAT_READ_SINGLE:

					if(a[pos] == '\''){
							curr = LAT_END;
							printf(",");
							break;
						}
						printf("%c", a[pos]);						
						break;

				case LAT_READ_DOUBLE:

					if(a[pos] == '\"'){
							curr = LAT_END;
							printf(",");
							break;
						}
						printf("%c", a[pos]);						
						break;
				case LAT_END:
					if(a[pos] =='l'){
						if(checkMatch(&a[pos], "lon")){
							curr = LON_FOUND;
							pos+= 2;
							break;
						}
					}
					break;

				case LON_CAND:
			//	printf("%c", a[pos]);

					if (a[pos] == '='){
						curr = LON_FOUND;
					}
					else if (!isspace(a[pos])){
						curr = TPRKT_START;
					}
					break;
				case LON_FOUND:
					if(a[pos] == '\''){
				//			printf("LAT_READ");
							curr = LON_READ_SINGLE;
						}
					else if(a[pos] == '\"'){
				//			printf("LAT_READ");
						curr = LON_READ_DOUBLE;
						}
					break;

				case LON_READ_SINGLE:
					if(a[pos] == '\''){
							curr = LON_END;
							printf(",");
							break;
						}
						printf("%c", a[pos]);
						
						break;

				case LON_READ_DOUBLE:
					if(a[pos] == '\"'){
							curr = LON_END;
							printf(",");
							break;
						}
						printf("%c", a[pos]);
						
						break;
				case LON_END:
					 if (a[pos] == '<'){
						if(checkMatch(&a[pos], "<ele") && ((a[pos+4] == '>') || isspace(a[pos+4]))){
							//printf("%s\n", &a[pos]);
							curr = ELE_FOUND;
							pos+=3;					
						}
					 }
					 break;


				case ELE_FOUND:
					if (a[pos] == '>'){
						curr = ELE_READING;
					//	printf("reached");
					}

					break;


				case ELE_READING:
					if (a[pos] == '<'){
						printf(",");
						curr = TIME_SEARCH;

					}
					else{
						printf("%c", a[pos]);
					}


					break;
				case TIME_SEARCH:
					if (a[pos] == '<'){
					
						if(checkMatch(&a[pos], "<time") && ((a[pos+5] == '>') || isspace(a[pos+5]))){
							

							curr = TIME_ELE_FOUND;
							
						}
					 }
					 break;

				case TIME_ELE_FOUND:
					if (a[pos] == '>'){
						curr = TIME_SCAN;

					}
					break;
				
				case TIME_SCAN:
					if(a[pos] == '<'){
						if(checkMatch(&a[pos], "</time")){
							printf("\n");
							curr = TPRK_DONE;
							
						}
					}
					else if (a[pos] == ','){
						printf("&comma;");

					}
						
						
					else {
						printf("%c", a[pos]);

					}

					break;
				case TPRK_DONE:
					if (a[pos] == '<'){
						if(checkMatch(&a[pos], "</trkpt")){
							curr = SCANNING;
						}
					}
			}
			pos++;
		}

		

		//printf("%d", checkMatch(&a[2], "<xml"));
}

void read_line(char s[], int max)
{
  int count = 0;
//  int i = 0;
  int ch;
  while ((ch = getchar()) != EOF)
    {
      if (count < max)

	{
	 //  	if (!isspace(ch)){
		// 	s[i] = ch;
		// 	i++;
		// }
		s[count] = ch;
	}
      count++;
    }
  if (count > max)
    {
      s[max] = '\0';
    }
  else
    {

      s[count] = '\0';
    }
}

bool checkMatch(char *MainString, char *subString){
	for (int i = 0; subString[i]; i++){
		
		if (tolower(MainString[i]) != subString[i]){
			return false;
		}
	}
	return true;
}


