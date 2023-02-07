if(status == -1) {
		perror("semctl() failed");
		exit(1);
	}