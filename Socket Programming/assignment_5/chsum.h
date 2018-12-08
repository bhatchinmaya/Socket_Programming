unsigned short chsum(uint8_t *addr, uint8_t count)
			{
			     unsigned long int sum = 0;
   			     unsigned short checksum = 0;

         				while( count > 1 )  
					   {
         				      sum += * (unsigned short *) addr++;
         				      count -= 2;
					      addr++; 
						
      				           }

      				    
      				        if( count > 0 )
      				         sum += * (unsigned char *) addr;

     				    
      				        while (sum>>16)
     				         sum = (sum & 0xffff) + (sum >> 16);

     			      checksum = ~sum;
			      return checksum; 
			 }


	




