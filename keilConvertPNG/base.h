extern	void _sys_exit(int return_code);

extern	int sendchar (int ch);												/* Write character to Serial Port    */
extern	int getkey (void);																/* Read character from Serial Port   */

extern	void sendhex (int hex);                   	/* Write Hex Digit to Serial Port  */
extern	void sendstr (char *p);                   	/* Write string */

extern	void printArr(int arr[], int n);
extern	void printDecimal(int num);
