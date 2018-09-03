Guideline To Use FileExplorer  (FE)
------------------------------------------------------------------------
Git Hub Repository :  https://github.com/ConnectNitish/FileExplorer.git
------------------------------------------------------------------------
File Name : makeFile 
------------------------------
a) Dependencies are displayed in makeFile.
b) Program can be "make run" or "./FileExplorerIndex"
------------------------------------------------------------------------
Functionalities Implemented : (Index are Provided based on Indexes present in assignment document) 
----------------------------------------
Normal Mode : 

1. a) List Of Files and Directories is being Displayed as per details shared in assignment. 
	b) Scrolling is implemented  if more than 26 items in the Directory.  
		First time 26 item will be displayed , if directory has more than this then increamental of one item will be displayed. 
		Ex. First Display :  1-26 item. 
				If cursor On last line and hit down arrow : 2-27 item will be displayed and so on. 
				Similarly , If cursor on first line ( 2nd item of the directory ) and hit up arrow key : 1-26 item will be displayed and so on. 
	c) File Explorer (FE) show the entries for "." and ".."
	d) User Will be able to Scroll up and Down in case of directory having item more than 26.
	
2. a) i) Next Directory Content is displayed on press of "Enter" key if it is directory.
		ii) Files are getting displayed using xdg-open on press of "Enter" key. 

3. a) User will be able to Go Back on left arrow key if he has navigated before in forward directories.
		In case user is at opened directory itself. On first left key press , he will not be able to move in backward direction.
	b) User will be able to Go Forward on Right arrow key on all directories.
		In case user is at opened directory itself for first time he will be not able to go to Current "." and Back ".." directories. 
		In case user is at intermediate directories , then he will not be able to go to anywhere on Current "." directory with right arrow key.
		In case user is at intermediate directories , then he will be able to go to anywhere on Back ".." directory with right arrow key. 
		But maximum back he can go till the point where the application is started.	 
	c) On pressing BackSpace he can go to Up One Level. 
	d) On Pressing 'h' user will be navigated directly to the folder from where the application is started.
		
Command Mode : 
On press of ":" user will be able to navigate from Normal Mode to Command Mode. 
Currently on press of "Esc+Entry" user will be able to navigate from Command Mode to Normal Mode. 
1. a) copy will work for single as well as multiple file as well as directories.
		Ex : copy qw.txt logs.txt ./abcd  								( abcd is folder inside the current directory )
				copy qw.txt logs.txt /home/abcd						( /home/abcd will be full qualified path)
		move will work for single as well as multiple file as well as directories.
		Ex : move qw.txt logs.txt ./abcd  								( abcd is folder inside the current directory )
				move qw.txt logs.txt /home/abcd						( /home/abcd will be full qualified path)			
		rename will work for single file as well as directory.
		Ex : rename old_name new_name 						(assuming the file or directory exist in current folder).
	c) Directory Copy implementation is also done (explained in above Point ).
	d) Permission are full permission given to folders.
2. a) create_file will create single as well as multiple file ; provided directory has rights in the folder where is it creating.
	Ex.  create_file newFile.txt newFile2.txt ./abcd  								( abcd is folder inside the current directory )
			create_file newFile.txt newFile2.txt /home/abcd						( /home/abcd will be full qualified path)
	b) create_dir will create single as well as multiple directory ; provided directory has rights in the folder where is it creating.
	Ex.  create_dir newFile.txt newFile2.txt ./abcd  								( abcd is folder inside the current directory )
			create_dir newFile.txt newFile2.txt /home/abcd						( /home/abcd will be full qualified path)
3. a) delete_file will delete single file ; provided directory has rights in the folder where is it executing.
	Ex.  delete_file newFile.txt   																( newFile.txt will be deleted if present in current folder )
			delete_file /home/abcd/newFile.txt												( /home/abcd/newFile.txt will be full qualified path of the file which need to be deleted )
	b) delete_dir will delete single file ; provided directory has rights in the folder where is it executing.
	Ex.  delete_dir newDir   																	( newDir directory will be deleted if present in current folder )
			delete_dir /home/abcd/newDir												( /home/abcd/newDir will be deleted if present in current folder  with all its internal file recursively )
4. Goto Not Implemented.
5. Search Not Implemented.
6. SnapShot will be created for the directory provided. 
	On the Folder where the application is started a text file with the specified name will be made having all details same as "ls -R" command.
	Provided that user rsunning the application has right to create the file in directory.
	Ex. 	snapshot . abc.txt 																		(snapshot of current directory will be created in file named abc.txt)
			snapshot /home/abcd abc.txt 													(snapshot of directory specified as second argument will be created in file named abc.txt)
7. On press of "Esc+Enter" , user will be able to navigate to Normal Mode.	

------------------------------------------------------------------------
Assumptions : 

a) Please open the terminal in full screen mode before launching application to give proper layout.
b) Max File Entries displayed will be 26 items.
c) For Command Mode , all the command will be written at last 3rd row from bottom.
d) Any Error or informative message while accessing the application will be displayed at bottom left of  the screen.
e) By Default the Focus of the cursor will be set on first entry of the directory.
f) Scroll will happen increamental.  
g) Commands only accepts proper inputs with alphabets  and certain special character like Period (".") , Underscore("_") , backslash ("/"). 
h) File Name max will be 25 character. 

------------------------------------------------------------------------
Others :

a) On press of "q" or "Q" , if user is in Normal Mode , application will stop and set focus to terminal.
b) If any informative message is displayed on screen then it will get vanish on next directory change.
c) As an when a command execution is completed , old command will vanish out and focus will be set on for typing next new command.

------------------------------------------------------------------------

Nitish Srivastava ( 2018201012	)
	