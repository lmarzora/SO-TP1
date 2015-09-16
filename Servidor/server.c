

static char* nurseS;
static char* nurseR;

int main (void )
{
	createServer();
	createNurses()

	while(1)
	{
		doServer();
	}

}

void createNurse() 
{		
	char* nurseS = "/tmp/nurseS";
	if ( access(nurseS, 0) == -1 && mknod(nurseS, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod  nurseS");
	char* nurseR = "/tmp/nurseR";
	if ( access(nurseR, 0) == -1 && mknod(nurseR, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod nurseR");
	
	//create 3 nurse threads
	
	return ;
}


void doNurse()
{
	//read from nurseR
	//heal
	//write in nurseS
}	


void doServer() 
{
	CONNECTION c; 
	c = acceptConnection();
	int pid = fork();
	if( pid == 0)
		handleConnection( c );
	closeConnection( c );
}

void handleConnection( CONNECTION c )
{
	//transport
	receivePacket( c , &p , sizeof PACKET );
	processPacket( p );
	//transport
	sendPacket( c , &p , sizeof PACKET );
	closeConnection( c );
	exit(1);
}

void processPacket( PACKET* p )
{
	int opc = p->opc;

	switch( opc ) 
	{
		case CURAR:
		{	
			//write nurseR;
			//read nurseS;
}			break;
		}
		case ADOPTAR:
		{
			//call adoptar;
			break;			
		}
		case REGALAR:
		{
			//call regalar;
			break;
		}
		default:
		{
			//error;
		}
	}
}
