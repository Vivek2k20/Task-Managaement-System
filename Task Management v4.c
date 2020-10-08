/*



AUTHOR : Vivek M R
Work Started on 29/3/2020



LAST UPDATED : 17:40 , 22/8/2020
VERSION : 4.0


WHAT'S NEW : 
		ADDED EXPORT OPTION!!!!
		Now you can export All tasks,Remaining Tasks and Completed Tasks to an external csv file.
		This file can be imported to google calendar and other external calendars!
		

KNOWN BUGS :
		Doesn't Work on Non-Windows Platform.This is because of some of the inbuilt functions and header files used.
		Doesn't Take Space in the input for Names of Task.Users are requested to use '_' instead of space to seperate 2 words in the Name of the Tasks
		while running the code.



*/






// Including Header Files//
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<unistd.h> 

//Few Global Variables to keep track of the sort configuration as well as No. of Remaining,Done and Total Tasks//
int rno,dno,no,sortby;

//Date Structure.Contains Day,Month,Year.Its used inside another task structure//
struct datestruct
{
int day,month,year;
};
/*
   Task Structure.Contains the following :
                  1.Task Name
                  2.Sl.No
                  3.Done status(Done:1,Not Done:0)
                  4.Due Date(Nesting the datestruct)
                  5.link field(Pointing to the next node)

 */
struct taskstruct
{
char name[10];
int sl;
int done;
struct datestruct date;
struct taskstruct *link;
};
typedef struct taskstruct *Node;


//Defining MALLOC to allocate Memory Dynamically//
#define MALLOC(p,s,t)\
p=(t)malloc(s);\
if(p==NULL)\
{\
printf("Insufficient Memory\nExiting...\n");\
getch();\
exit;\
}

/*
   Add Function.Adds Multiple/Single Task to the Task List.
   All Appropriate Inputs about the task is taken from user
   and linked at the end of the list.
*/
Node add(Node start)
{
	printf("How many tasks do you want to add?\n");
	int n;
	scanf("%d",&n);
	if(n<1)
	{
		printf("Add Operation Unsuccessful (No. of Tasks cannot be Negative or Zero).\n");
		return start;
	};
	int i;
	//Adding n No. Of Tasks
	for(i=0;i<n;i++)
	{
		Node temp,q;
		//Dynamically Allocating Memory for the new task
		MALLOC(temp,sizeof(struct taskstruct),Node);
		//Traversing to the end of List
		if(no==0)
			start=temp;
		else
		{
			q=start;
			while(q->link!=NULL)
				q=q->link;
			q->link=temp;
		};
		//Initializing all the data members of the new task to a default value.
		temp->date.day=temp->date.month=temp->date.year=0;
		temp->link=NULL;
		temp->sl=no+1;
		temp->done=0;
		//Taking input for the name of the task
		printf("Enter Task %d info :\nName:\n",no+1);
		scanf("%s",temp->name);
		printf("Enter Due Date :\n");
		/*
		   invalid variable is used as a flag to check the validity of the entered input
		   for due date.
		   In order to check the validity of the entered dates,we need several informations
		   like if the entered year is a leap year,if the entered month has 28/29/30/31 days.
		   leap is a variable used as a flag whenever the year entered is a leap year.
		   When the Input is a Leap Year,Leap=1 else 0.
		   Similarly,lessdays is variable that is used to decide what is range of the valid days
		   depending on the month and year.
		   The Values of lessdays for several configurations are given below:
		   When lessdays=0,The Month contains 31 days.
		   When lessdays=1,The Month contains 30 days.
		   When lessdays=2,The Month is a February.Hence Depending on the value of leap,
		   The Month contains 28/29 days.
		*/
		int invalid=1;
		int leap=0;
		int lessdays=0;
		//While Loop used to force the user to give a valid date.
		while(invalid)
		{
			printf("Enter Year :\n");
			scanf("%d",&temp->date.year);
			//Checking if Entered Year is a Leap Year and Updating leap variable accordingly.
			if(((temp->date.year)%400==0)||((temp->date.year%100!=0)&&(temp->date.year%4==0)))
				leap=1;
			else leap=0;
			printf("Enter Month :\n");
			scanf("%d",&temp->date.month);
			/*
			  Checking if the Entered Month is valid(Between 1 and 12) and updating invalid accordingly
			  Skipping the rest of the while loop to take a fresh input if its invalid
			*/
			if((temp->date.month>0)&&(temp->date.month<13))
				invalid=0;
			else
			{
				printf("Invalid Month\n");
				invalid=1;
				continue;
			};
			/*
			  Updating the Value of lessdays depending on the month
			 */
			if(temp->date.month<8)
			{
				if(temp->date.month%2==0)
					lessdays=1;
				else lessdays=0;
			}else
			{
				if(temp->date.month%2==1)
					lessdays=1;
				else lessdays=0;
			};
			if(temp->date.month==2)
				lessdays=2;
			printf("Enter day:\n");
			scanf("%d",&temp->date.day);
			/*
			 * Checking if the day is valid
			 */
			if((temp->date.day>0))
			{
				if((lessdays==0)&&(temp->date.day<32))
						invalid=0;
				else if((lessdays==1)&&(temp->date.day<31))
						invalid=0;
				else if(lessdays==2)
				{
					if((leap==0)&&(temp->date.day<29))
							invalid=0;
					else if((leap==1)&&(temp->date.day<30))
							invalid=0;
					else
					{
					invalid=1;printf("Invalid Day\n");continue;
					};
				}else
					{
					invalid=1;printf("Invalid Day\n");continue;
					};
			}else
				{
					invalid=1;
					printf("Invalid Day\n");
					continue;
				};
			};rno++;no++;
	};printf("%d Tasks Added.\n",n);return start;
}


/*
  display Function is used to display either the remaining tasks or completed tasks,but not both.
  When config=0,Remaining Tasks are Displayed.
  When config=1,Completed Tasks are Displayed.

 */
void display(Node start,int config)
{
	if((rno==0)&&(config==0))
	{
		printf("No Remaining Tasks left\n");
		return;
	}
	else if((dno==0)&&(config==1))
	{
		printf("No Task Completed\n");
		return;
	};
	Node q=start;
	displaysortby();
	printf("Sl.No\t\tTask Name\t\tDue Date\n");
	while(q!=NULL)
	{
		if(q->done==config)
		printf("%d\t\t%s\t\t\t%d/%d/%d\n",q->sl,q->name,q->date.day,q->date.month,q->date.year);
		q=q->link;
	};
}

/*
  displaysortby Function is used to display in what way the current list is sorted.
 */
void displaysortby()
{
	switch(sortby)
	{
	case 1 :printf("\nDisplaying According to Name (Ascending) :\n");break;
	case 2 :printf("\nDisplaying According to Name (Descending) :\n");break;
	case 3 :printf("\nDisplaying According to Due Date (Urgent First/Ascending) :\n");break;
	case 4 :printf("\nDisplaying According to Due Date (Urgent Last/Descending) :\n");break;
	case 5 :printf("\nDisplaying According to First Created (Sl.No Ascending) :\n");break;
	default :printf("\nDisplaying According to Last Created (Sl.No Descending) :\n");break;
	};
}

/*
 edit Function is used to edit a single Remaining Task. It cannot be used to edit an already Completed Task.
 It also cannot be used to edit sl.no or done status of the task.Only Name and Due Date of the Tasks can be Edited.
 a is the sl.no of the task that is to be edited.
 */
Node edit(Node start,int a)
{
	Node q=start;
	//Traversing till the required task/node is found.
	while((q->sl!=a)&&(q!=NULL))
		{
			q=q->link;
		};
	//Checking if the task is completed and updating q to NULL if it is completed.
	if(q->done==1)
		q=NULL;
	if(q==NULL)
	{
		printf("Invalid Sl.No\n");
		return start;
	};
	//Asking what field of the task is to be updated from the user.
	printf("1.Name\n2.Due Date\n3.Both\nEnter the field to be edited :\n");
	int ch;
	int invalid=1;
    int leap=0;
    int lessdays=0;
	scanf("%d",&ch);
	//Editing the field(s) of the task.
	switch(ch)
	{
	case 1: printf("Enter New Name :\n");
			scanf("%s",q->name);printf("Name of Task %d changed to %s.\n",q->sl,q->name);break;
	case 2: printf("Enter Due Date :\n");
			while(invalid)
			{
				printf("Enter Year :\n");
				scanf("%d",&q->date.year);
				if(((q->date.year)%400==0)||((q->date.year%100!=0)&&(q->date.year%4==0)))
					leap=1;
				else leap=0;
				printf("Enter Month :\n");
				scanf("%d",&q->date.month);
				if((q->date.month>0)&&(q->date.month<13))
					invalid=0;
				else
				{
					printf("Invalid Month\n");
					invalid=1;
					continue;
				};
				if(q->date.month<8)
				{
					if(q->date.month%2==0)
						lessdays=1;
					else lessdays=0;
				}else
				{
					if(q->date.month%2==1)
						lessdays=1;
					else lessdays=0;
				};
				if(q->date.month==2)
					lessdays=2;
				printf("Enter day:\n");
				scanf("%d",&q->date.day);
				if((q->date.day>0))
				{
					if((lessdays==0)&&(q->date.day<32))
						invalid=0;
					else if((lessdays==1)&&(q->date.day<31))
						invalid=0;
					else if(lessdays==2)
					{
						if((leap==0)&&(q->date.day<29))
							invalid=0;
						else if((leap==1)&&(q->date.day<30))
							invalid=0;
						else
						{
							invalid=1;printf("Invalid Day\n");continue;
						};
					}else
					{
						invalid=1;printf("Invalid Day\n");continue;
					};
				}else
				{
					invalid=1;
					printf("Invalid Day\n");
					continue;
				};
			};printf("Due Date of Task %d Updated to %d/%d/%d.\n",q->sl,q->date.day,q->date.month,q->date.year);break;
	case 3: printf("Enter New Name :\n");
			scanf("%s",q->name);
			printf("Enter Due Date :\n");
			while(invalid)
			{
				printf("Enter Year :\n");
				scanf("%d",&q->date.year);
				if(((q->date.year)%400==0)||((q->date.year%100!=0)&&(q->date.year%4==0)))
					leap=1;
				else leap=0;
				printf("Enter Month :\n");
				scanf("%d",&q->date.month);
				if((q->date.month>0)&&(q->date.month<13))
					invalid=0;
				else
				{
					printf("Invalid Month\n");
					invalid=1;
					continue;
				};
				if(q->date.month<8)
				{
					if(q->date.month%2==0)
						lessdays=1;
					else lessdays=0;
				}else
				{
					if(q->date.month%2==1)
						lessdays=1;
					else lessdays=0;
				};
				if(q->date.month==2)
					lessdays=2;
				printf("Enter day:\n");
				scanf("%d",&q->date.day);
				if((q->date.day>0))
				{
					if((lessdays==0)&&(q->date.day<32))
						invalid=0;
					else if((lessdays==1)&&(q->date.day<31))
						invalid=0;
					else if(lessdays==2)
					{
						if((leap==0)&&(q->date.day<29))
							invalid=0;
						else if((leap==1)&&(q->date.day<30))
							invalid=0;
						else
						{
							invalid=1;printf("Invalid Day\n");continue;
						};
					}else
					{
						invalid=1;printf("Invalid Day\n");continue;
					};
				}else
				{
					invalid=1;
					printf("Invalid Day\n");
					continue;
				};
			};
			printf("Task %d has been Updated.\nName changed to %s.\n",q->sl,q->name);
			printf("Due Date changed to %d/%d/%d.\n",q->sl,q->date.day,q->date.month,q->date.year);
			break;
	default:printf("Edit Operation Unsuccessful (Invalid choice)\n");
	};return start;
}


/*
   done Function is used to change the done status of the remaining tasks(Incomplete Tasks) to 1(Complete)
 */
Node done(Node start,int a)
{
	Node q=start;
	while((q->sl!=a)&&(q!=NULL))
		q=q->link;
	if(q->done==1)
		q=NULL;
	if(q==NULL)
	{
		printf("Operation Unsuccessful (Invalid Sl.No)\n");
		return start;
	};
	q->done=1;
	printf("Task %d Marked as Done.\n",a);
	dno++;
	rno--;
	return start;
}


/*
   deletetask Function is used to Delete any one task. The Task can be Completed/Incomplete.
   The Required Link changes are done while deleting the task and no is decremented
   Depending on whether the task that is deleted is a Completed/Uncompleted Task,rno or dno is decremented.
   The Sl.Nos of the Tasks are Altered to bring in Uniformity and easier Addition of Future Tasks.
   This is done by decrementing the sl.nos of all the tasks that were created(Having Higher Sl.No than the deleted task) after the task
   that is deleted.
 */
Node deletetask(Node start,int a)
{
	Node p,q;
	if(a==1)
	{
		p=NULL;
		q=start;
	}else
	{
		q=start->link;
		p=start;
		while(q->sl!=a)
		{
		q=q->link;
		p=p->link;
		};
	};
	printf("Sl.No\t\tTask Name\t\tDue Date\t\tComplete\n");
	printf("%d\t\t%s\t\t\t%d/%d/%d\t\t",q->sl,q->name,q->date.day,q->date.month,q->date.year);
	if(q->done==0)
		printf("Yes\n");
	else printf("No\n");
	printf("Are you sure you want to delete this task?\n1.Yes\n2.No\n");
	int ch;
	scanf("%d",&ch);
	if(ch==2)
		printf("Deletion Unsuccessful (Stopped By the User).Press any key to return to MENU...\n");
	else if(ch==1)
	{
		if(a!=1)
			p->link=q->link;
		else start=q->link;
		if(q->done==1)
			dno--;
		else rno--;
		free(q);
		no--;
		p=start;
		while(p!=NULL)
		{
			if(p->sl>a)
				p->sl--;
			p=p->link;
		};
		printf("Task %d Deleted Successfully.\nSl.Nos of the tasks have been updated accordingly.\nPress any key to return to MENU...\n",a);
	}else
		printf("Deletion Unsuccessful (Stopped By The System due to the invalid input by the user)\Press any key to return to MENU...\n");
	return start;
}








/*
 Changes Links and Sorts both a and b Sub-Lists and Merges the 2 sub Lists.
*/
Node SortedMerge(Node a, Node b)
{
    Node result = NULL;


    /*
      Base cases
      */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    /*
     Pick either a or b depending on sortby value, and recur
    */
    switch(sortby)
    {
    case 1: if (strcmpi(b->name,a->name)>=0)
   	   	    {
       	    result = a;
       	    result->link = SortedMerge(a->link, b);
   	   	    }
   	   	    else
   	        {
   	   	    result = b;
   	   	    result->link = SortedMerge(a, b->link);
   	   	    };
   	   	    break;
    case 2: if (strcmpi(a->name,b->name)>=0)
   	    	{
	   	    result = a;
	   	    result->link = SortedMerge(a->link, b);
   	   	    }
   	   	    else
   	   	    {
   	   	    result = b;
   	   	    result->link = SortedMerge(a, b->link);
   	   	    };
   	   	    break;
    case 3: if (a->date.year < b->date.year)
   	   	    {
	   	    result = a;
	   	    result->link = SortedMerge(a->link, b);
   	   	    }
   	   	    else if(a->date.year > b->date.year)
   	   	    {
   	   	    result = b;
   	   	    result->link = SortedMerge(a, b->link);
   	   	    }
   	   	    else if (a->date.month < b->date.month)
   	   	    {
	   	    result = a;
	   	    result->link = SortedMerge(a->link, b);
   	   	    }
   	   	    else if(a->date.month > b->date.month)
   	   	    {
   	   	    result = b;
   	   	    result->link = SortedMerge(a, b->link);
   	   	    }
   	   	    else if (a->date.day <= b->date.day)
   	   	    {
	   	    result = a;
	   	    result->link = SortedMerge(a->link, b);
   	   	    }
   	   	    else
   	   	    {
   	   	    result = b;
   	   	    result->link = SortedMerge(a, b->link);
   	   	    };
    		break;
    case 4: if (a->date.year > b->date.year)
    		{
    		result = a;
    		result->link = SortedMerge(a->link, b);
    		}
  	    	else if(a->date.year < b->date.year)
  	    	{
  	    	result = b;
  	    	result->link = SortedMerge(a, b->link);
  	    	}
  	    	else if (a->date.month > b->date.month)
  	    	{
  	    	result = a;
  	    	result->link = SortedMerge(a->link, b);
  	    	}
  	    	else if(a->date.month < b->date.month)
  	    	{
  	    	result = b;
  	    	result->link = SortedMerge(a, b->link);
  	    	}
  	    	else if (a->date.day >= b->date.day)
  	    	{
  	    	result = a;
  	    	result->link = SortedMerge(a->link, b);
  	    	}
  	    	else
  	    	{
  	    	result = b;
  	    	result->link = SortedMerge(a, b->link);
  	    	};
   	   	    break;
    case 5: if (a->sl <= b->sl)
    		{
    		result = a;
    		result->link = SortedMerge(a->link, b);
    		}
    		else
    		{
    		result = b;
    		result->link = SortedMerge(a, b->link);
    		};
    		break;
   default: if (a->sl >= b->sl)
    		{
    		result = a;
    		result->link = SortedMerge(a->link, b);
    		}
    		else
    		{
    		result = b;
    		result->link = SortedMerge(a, b->link);
    		};
    };
    return (result);
}




/*
 	Split the nodes of the given list into front and back halves,
    and return the two lists using the reference parameters.
    If the length is odd, the extra node should go in the front list.
    Uses the fast/slow pointer strategy to determine the midpoint of the list.
*/
void FrontBackSplit(Node source,
                    Node* frontRef, Node* backRef)
{
    Node fast;
    Node slow;
    slow = source;
    fast = source->link;

    /*
     Advance 'fast' by two nodes, and Advance 'slow' by one node
      */
    while (fast != NULL) {
        fast = fast->link;
        if (fast != NULL) {
            slow = slow->link;
            fast = fast->link;
        }
    }

    /*
     'slow' is before the midpoint in the list, so split it in two
    at that point.
     */
    *frontRef = source;
    *backRef = slow->link;
    slow->link = NULL;
}



/*
  Sorts the linked list by changing link pointers itself (not data).
  The Method used is Merge Sort since it is very efficient at sorting Linked Lists.
  */
void MergeSort(Node* headRef)
{
    Node head = *headRef;
    Node a;
    Node b;

    /*
     Base case = length 0 or 1
     */
    if ((head == NULL) || (head->link == NULL)) {
        return;
    }

    /*
     Split head into 'a' and 'b' sublists
     */
    FrontBackSplit(head, &a, &b);

    /*
      Recursively sort the sublists
      */
    MergeSort(&a);
    MergeSort(&b);

    /*
      answer = merge the two sorted lists together
     */
    *headRef = SortedMerge(a, b);
}






/*
 displayall Function is Used to Display the info of all the tasks(Both Complete and Incomplete
 */
void displayall(Node start)
{
	Node q=start;
	if(no==0)
	{
		printf("No Tasks Present in the List.\n");
		return;
	};
	displaysortby();
	printf("Sl.No\t\tTask Name\t\tDue Date\t\tComplete\n");
		while(q!=NULL)
		{
			printf("%d\t\t%s\t\t\t%d/%d/%d\t\t",q->sl,q->name,q->date.day,q->date.month,q->date.year);
			if(q->date.year<1000)
				printf("\t");
			if(q->done==1)
				printf("Yes\n");
			else printf("No\n");
			q=q->link;
		};
}

/*
 clearall Function is Used to Clear/Delete All the Tasks in the List and Initialize the values of
 rno,dno,no,sortby and start.
 */
Node clearall(Node start)
{
	Node p,q;
	p=start;
	while(p!=NULL)
	{
		q=p->link;
		free(p);
		p=q;
	};
	start=NULL;
	rno=no=dno=0;
	sortby=5;
	return start;
}



void export(Node start)
{
	printf("WARNING!!\nYou can export the tasks to a new file or overwrite an existing file.\n");
	printf("To Export and Overwrite to an existing file,Enter the filename and its directory.\nHowever if the file doesn't exist,A new file will be created\n");
	Node q=start;
	int checkname=0;
	int confirm=0;
	FILE *fp;
	char fname[100];
	char *dir=(char *)malloc(100* sizeof(char));
	dir=getcwd(dir,100);
	while(!(checkname))
	{
		printf("Enter Filename to be exported to :\n");
		scanf("%s",fname);
		printf("Filename : %s\nIf the file already exists,it will be overwritten\nConfirm?\n1.Yes\n2.No\nEnter your choice:\n",fname);
		scanf("%d",&checkname);
		if(checkname==2)
			checkname=0;
		else if(checkname!=1)
		{
			printf("Export Operation aborted due to invalid choice.\nPress any key to return to MENU...\n");
			getch();
			return;
		};
	};
	printf("The File will be created in the same folder/path as the source code.\nThe Current Directory/Path where the file will be created is : %s\nDo you want to change this?\n1.Yes\n2.No\nEnter your choice:\n",dir);
	scanf("%d",&confirm);
	if(confirm==1)
	{
		printf("Enter the new path where the file should be created :\n");
		gets(dir);	
		if(chdir(dir)!=0)
		{
			perror("Export Operation failed. Directory error");
			printf("\nPress any key to return to MENU...\n");
			getch();
			return;
		};
	}else if(confirm!=2)
	{
		printf("Export Operation aborted due to invalid choice.\nPress any key to return to MENU...\n");
		getch();
		return;
	};
	strcpy(fname,strcat(fname,".csv"));
	fp=fopen(fname,"w+");
	printf("Select Tasks to Export :\n1.All Tasks\n2.Only Completed Tasks\n3.Only Remaining Tasks\n");
	int choice;
	scanf("%d",&choice);
	if(choice==1)
	{
		fprintf(fp,"Subject,Start date,All Day Event\n");
		while(q!=NULL)
		{
			fprintf(fp,"%s,%d/%d/%d,True\n",q->name,q->date.day,q->date.month,q->date.year);
			q=q->link;
		};
		fclose(fp);
		printf("Export Operation Successful!\n%s created at %s!\n",fname,dir);
		printf("Press any key to return to MENU...\n");
		getch();
		return;
	}else if((choice!=2)&&(choice!=3))
	{
		printf("Export Operation aborted due to invalid choice.\nPress any key to return to MENU...\n");
		getch();
		return;
	}else 
	{
		int attr;
		if(choice==2)
		{	
			if(dno==0)
			{
				printf("No Tasks Present in the List.\nExport Operation Aborted.\nPress any key to return to MENU...\n");
				return;
			};
			attr=1;
		}else if(choice==3)
		{
			if(rno==0)
			{
			printf("No Tasks Present in the List.\nExport Operation Aborted.\nPress any key to return to MENU...\n");
			return;
			};
			attr=0;
		};
		fprintf(fp,"Subject,Start date,All Day Event\n");
		while(q!=NULL)
		{
			if(q->done==attr)
				fprintf(fp,"%s,%d/%d/%d,True\n",q->name,q->date.day,q->date.month,q->date.year);
			q=q->link;
		};
		fclose(fp);
		printf("Export Operation Successfull!\n%s created at %s!\n",fname,dir);
		printf("Press any key to return to MENU...\n");
		getch();
		return;
	};
	
}












//Main Function//
void main()
{
	Node start;
	MALLOC(start,sizeof(struct taskstruct),Node);
	start=NULL;
	rno=dno=no=0;
	sortby=5;
	int ch;
	int subch;
	while(1)
	{
		printf("\nMENU:\n\n1.Add Tasks\n2.Edit Remaining Tasks\n3.Mark a Task as done\n4.Delete a Task\n5.View Tasks\n6.Sort Tasks\n7.Clear All Tasks\n8.Export to an external CSV file\n9.Exit\n\nEnter your choice :\n");
		scanf("%d",&ch);
		switch(ch)
		{
		case 1: start=add(start);
		/*
		  	When Tasks are Added,By Default Adding is done at the end of the list.
		  	If this isn't in complaince with the value of sortby,and display operation is used right
		    after addition,it might look like the sorting is incorrectly done.
		    Hence , Calling MergeSort Function Everytime New Task is Added.
		 */
				MergeSort(&start);
				printf("Press any key to return to MENU...\n");
				getch();
				break;
		case 2:	if(rno==0)
				{
					printf("No Remaining Tasks.\n");
				}
				else
				{
					display(start,0);
					printf("Enter the Sl.No of the task to be edited :\n");
					scanf("%d",&subch);
					edit(start,subch);
				};
				printf("Press any key to return to MENU...\n");
				getch();
				break;
		case 3: if(rno==0)
				{
					printf("No Remaining Tasks.\n");
				}
				else
				{
					display(start,0);
					printf("Enter the Sl.No of the task to be marked as done :\n");
					scanf("%d",&subch);
					start=done(start,subch);
				};
				printf("Press any key to return to MENU...\n");
				getch();
				break;
		case 4: if(no!=0)
				{
				displayall(start);
				printf("Enter the Sl.No of the task to be Deleted :\n");
				scanf("%d",&subch);
				if((subch>no)||(subch<=0))
				{
					printf("Deletion Unsuccessful (Invalid Sl.No).\nPress any key to return to MENU...\n");getch();
					break;
				};
				start=deletetask(start,subch);
				/*
				When All Tasks are deleted individually(one after another) ,the value of no becomes 0.
				So,Again On Addition on new tasks,the old sortby value is used to display.
				Hence,Setting sortby to 5 whenever no=0.
				 */
				if(no==0)
					sortby=5;
				}
				else
					printf("No Tasks left to delete!\nPress any key to return to MENU...\n");
				getch();
				break;
		case 5:	printf("No. of Remaining Tasks : %d\nNo. of Completed Tasks : %d\nTotal No. of Tasks : %d\n1.View Remaining Tasks\n2.View Completed Tasks\n3.View Both\nEnter your choice :\n",rno,dno,no);
				scanf("%d",&subch);
				if(subch==1)
					display(start,0);
				else if(subch==2)
					display(start,1);
				else if(subch==3)
					displayall(start);
				else printf("Display Operation Unsuccessful (Invalid Choice).\n");
				printf("Press any key to return to MENU...\n");
				getch();
				break;
		case 6: printf("1.Sort By Name (Ascending)\n2.Sort By Name(Descending)\n3.Sort By Due Date(Urgent First/Ascending)\n4.Sort By Due Date(Urgent Last/Descending)\n5.Sort By Creation(Ascending)\n6.Sort By Creation(Descending)\nEnter your choice :\n");
				scanf("%d",&subch);
				if((subch<1)||(subch>6))
					printf("Sorting Unsuccessful (Invalid Choice).\n");
				else
				{
					sortby=subch;
					MergeSort(&start);
					printf("Sorting Successful.\n");
				};
				printf("Press any key to return to MENU...\n");
				getch();
				break;
		case 7: printf("Are you sure you want to clear all tasks?\nWARNING : This will clear ALL tasks and ALL OF THEIR DETAILS.\nContinue?\n1.Yes\n2.No\nEnter your choice : \n");
				scanf("%d",&subch);
				if(subch==1)
				{
					start=clearall(start);
					printf("All Tasks Cleared!\n");
				}else if(subch==2)
				{
					printf("Tasks Not Cleared!\nOperation Aborted by the User.\n");
				}else printf("Tasks Not Cleared!\nOperation Aborted due to invalid response to the confirmation prompt by the User!\n");
				printf("Press any key to return to MENU...\n");
				getch();
				break;
		case 8: if(no==0)
				{
					printf("No Tasks to Export..\nPress any key to return to MENU...\n");
					getch();
					break;
				};
				while(subch)
				{
					printf("Currently,The Order of Tasks is :\n");
					displayall(start);
					printf("Export the tasks in current order?\n1.Yes\n2.No\nEnter your choice:\n");
					scanf("%d",&subch);
					if(subch==2)
					{
						printf("Select the new order :\n");
						printf("1.Sort By Name (Ascending)\n2.Sort By Name(Descending)\n3.Sort By Due Date(Urgent First/Ascending)\n4.Sort By Due Date(Urgent Last/Descending)\n5.Sort By Creation(Ascending)\n6.Sort By Creation(Descending)\nEnter your choice :\n");
						scanf("%d",&subch);
						if((subch<1)||(subch>6))
							printf("Sorting Unsuccessful (Invalid Choice).\n");
						else
						{
							sortby=subch;
							MergeSort(&start);
							printf("Sorting Successful.\n");
						};
						subch=1;
					}else if(subch==1)
						subch=0;
					else break;
				};
				if(subch!=0)
				{
					printf("Export Operation aborted due to invalid choice.\nPress any key to return to MENU...\n");
					getch();
					break;
				};
				export(start);
				break;
		case 9: printf("Clearing Memory and Exiting...\n");
				start=clearall(start);
				free(start);
				printf("Memory Cleared!\n");
				printf("Press any key to exit....");
				getch();
				exit(0);
		default:printf("Invalid Choice.Press any key to return to MENU...\n");
				getch();
		};
	};
}


