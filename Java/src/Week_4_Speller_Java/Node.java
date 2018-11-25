package Week_4_Speller_Java;

public class Node
{
	public boolean is_word; // indicates whether a current letter is a valid word
	public Node[] children; // pointer to a table that contains the next element (letter) of the word

	public Node() // in C#/Java we can use a constructor to initialize the necessary items
	{
		is_word = false;
		children = new Node[27];
	}
}
