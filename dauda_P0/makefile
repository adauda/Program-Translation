CC=gcc

PO: main.o buildTree.o traversals.o
	$(CC) -o PO main.o buildTree.o traversals.o

clean:
	rm PO *.o *.preorder *.inorder *.postorder 

.SUFFIXES: .o .c .preorder .inorder .postorder

.c.o:
	$(CC) -c $<
