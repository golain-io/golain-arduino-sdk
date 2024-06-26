# Data-Points Integration Guide

This guide outlines the steps required to integrate Data-Points functionality using the provided data-points.proto file. 
Follow these instructions carefully to generate and use the necessary header files for your project.

### Prerequisites

- Access to the Golain console.
- A development environment set up with the necessary tools for working with protocol buffers.

### Steps

1. **Generate Header Files**

	1.	Access the Data-Points Section:
	•	Log in to the Golain console.
	•	Navigate to the Data-points section.
	2.	Edit the data-points.proto File:
	•	Use the interface provided to edit the data-points.proto file.
	3.	Generate Headers:
	•	Follow the on-screen instructions to generate the header files.
	•	The system will process the data-points.proto file and generate the corresponding headers.

2. **Download the Generated Files**

	1.	Locate the Files:
	•	Once the generation process is complete, download the header files from the provided `Download NanoPB Header Files` button.
	•	The downloaded files should include <data-point_name>.pb.h and <data-point_name>.pb.c.
	2.	Verify Downloads:
	•	Ensure that both <data-point_name>.pb.h and <data-point_name>.pb.c are present in your downloads.

3. **Integrate Header Files into Your Project**

	1.	Move Files to Main Folder:
	•	Copy <data-point_name>.pb.h and <data-point_name>.pb.c from your downloads folder.
	•	Paste these files into the main directory of your project where your primary source files reside.
	2.	Include Headers in Your Code:
	•	In your main source file(s), include the headers using the following lines: `#include "<data-point_name>.pb.h"`

4.  **Compile and Test:**
	1. 	Ensure that your Arduino IDE is configured to compile the .c file and link it properly.
	2.	Compile your project and run tests to verify that the Data-Points functionality is working as expected by 
changing the inputs on the dashboard and they are reflected on the device.


### Conclusion

By following these steps, you will successfully integrate Data-Points capabilities into your project. Ensure that you 
periodically check for updates or changes in the Golain console that might affect the integration process.