package Week_4_Speller_Java;

import java.io.*;


public class Speller
{
	//Defining global parameters
	private static Node root; // handle for the TRIE
	private static int dict_size; // extra global item for counting the dictionary size while importing
	private static final int LENGTH = 45; // Maximum length for a word set as constant 45 alphabetical letters


	// Returns true if word is in dictionary else false
	private static boolean Check(String word)
	{
		Node ptr = root; // pointer to traverse the TRIE, no need for memory allocation

		for (int idx = 0; idx < word.length(); idx++)
		{
			int key = (word.charAt(idx) == '\'') ? 26 : Character.toLowerCase(word.charAt(idx)) - 97; // allocate the apostrophe inside children at location 26

			if (ptr.children[key] == null)
				return false; // as soon as we come across a NULL, we break the search immediately

			ptr = ptr.children[key]; // else, we go down deeper
		}

		if (ptr.is_word == true) // at this point we know that the path exists, so depending on the flag we return either true or false
			return true;
		else
			return false;
	}


	// Loads dictionary into memory, returning true if successful else false
	private static boolean Load(String dictionary) throws IOException
	{
		// Try to open text
		InputStreamReader file; // initialize a stream reader due to exception handling
		try 
		{
			file = new InputStreamReader(new FileInputStream(dictionary));
		}
		catch (java.lang.Exception e) 
		{
			return false;
		}

		root = new Node(); // initialize the root - this is a handle to the TRIE
		Node cursor; // pointer for the current node
		int i = file.read();
		char c; // variable to store char as we are going to scan the text character by character

		while (i != -1) // we scan the whole file until we get to EOF
		{
			cursor = root; // each time we start a new word, the pointer to the root has to be reinitialized to the root
			c = (char)i;

			while (true) // this is the inner loop for a single word in each line
			{
				if (c == '\n') // this is the only way to stop the inner loop - with the end of current line
				{
					cursor.is_word = true;
					dict_size++;
					break;
				}

				int key = (c == '\'') ? 26 : c - 97; // allocate the apostrophe inside children at location 26

				if (cursor.children[key] != null) // if the given character already exists, then we just go one step down
				{
					cursor = cursor.children[key];
				}
				else
				{
					cursor.children[key] = new Node(); // if not, before we go down the node, it has to be initialized
					cursor = cursor.children[key];
				}

				c = (char)file.read(); // moving onto a next character
			}
			i = file.read();
		}

		file.close();

		return true;
	}

	// Returns number of words in dictionary if loaded else 0 if not yet loaded
	private static int Size()
	{
		return dict_size;
	}

	
	public static void main(String[] args) throws IOException 
	{
		// Determine the dictionary and text file to be analyzed
		String dictionary = "Files/dictionary"; // imports a dictionary name, depending if a non-standard was used
		String text = "iles/lalaland.txt";
		InputStreamReader file; // initialize a stream reader due to exception handling

		// Structures for timing data
		long startTime, endTime;
		double time_load = 0.0, time_check = 0.0, time_size = 0.0;

		// Load dictionary 
		startTime = System.nanoTime();
		boolean loaded = Load(dictionary);
		endTime = System.nanoTime();

		// Exit if dictionary not loaded
		if (!loaded)
		{
			System.out.printf("Could not load %s.\n", dictionary);
			return;
		}

		// Calculate time to load dictionary
		time_load = (double)(endTime - startTime) / 1_000_000_000.0;

		// Try to open text
		try
		{
			file = new InputStreamReader(new FileInputStream(text));
		}
		catch (java.lang.Exception e)
		{
			System.out.printf("Could not open %s.\n", text);
			return;
		}

		// Prepare to report misspellings
		System.out.print("\nMISSPELLED WORDS\n\n");

		// Prepare to spell-check
		int index = 0; // 'index' is an indicator that tells us how long the current word is
		int misspellings = 0;
		int words = 0;
		StringBuilder sb = new StringBuilder();
		String word = "";
		char c;

		try // Spell-check each word in text
		{
			for (int i = file.read(); i != -1; i = file.read())
			{
				c = (char)i;
				// Allow only alphabetical characters and apostrophes
				if ((Character.toLowerCase(c) >= 'a' && Character.toLowerCase(c) <= 'z') || (c == '\'' && index > 0))
				{
					// Append character to word
					sb.append(c);
					index++;

					// Ignore alphabetical strings too long to be words
					if (index > LENGTH)
					{
						// Consume remainder of alphabetical string
						do
						{
							c = (char)file.read();
						} while (c != -1 && (Character.toLowerCase(c) >= 'a' && Character.toLowerCase(c) <= 'z'));

						// Prepare for new word
						index = 0;
						sb.setLength(0);
					}
				}

				// Ignore words with numbers (like MS Word can)
				else if (Character.isDigit(c))
				{
					// Consume remainder of alphanumeric string
					do
					{
						c = (char)file.read();
					} while (c != -1 && ((Character.toLowerCase(c) >= 'a' && Character.toLowerCase(c) <= 'z') || Character.isDigit(c))); // continue while alphanumeric characters

					// Prepare for new word
					index = 0;
					sb.setLength(0);
				}

				// If it's not a letter nor a digit, we must have found a whole word
				else if (index > 0)
				{
					// Terminate current word
					word = sb.toString();

					// Update counter
					words++;

					// Check word's spelling
					startTime = System.nanoTime();
					boolean misspelled = !Check(word);
					endTime = System.nanoTime();

					// Update benchmark
					time_check += ((double)(endTime - startTime) / 1_000_000_000.0);

					// Print word if misspelled
					if (misspelled)
					{
						System.out.println(word);
						misspellings++;
					}

					// Prepare for next word
					index = 0;
					sb.setLength(0);
				}
			}
		}

		catch (RuntimeException e) // Check whether there was an error while reading the file
		{
			file.close();
			System.out.println(e.getMessage());
			return;
		}

		// Close text
		file.close();

		// Determine dictionary's size
		startTime = System.nanoTime();
		int n = Size();
		endTime = System.nanoTime();

		// Calculate time to determine dictionary's size
		time_size = (double)(endTime - startTime) / 1_000_000_000.0;

		// Report benchmarks
		System.out.printf("\nWORDS MISSPELLED:     %d\n", misspellings);
		System.out.printf("WORDS IN DICTIONARY:  %d\n", n);
		System.out.printf("WORDS IN TEXT:        %d\n", words);
		System.out.printf("TIME IN load:         %.2f\n", time_load);
		System.out.printf("TIME IN check:        %.2f\n", time_check);
		System.out.printf("TIME IN size:         %.2f\n", time_size);
		System.out.printf("TIME IN TOTAL:        %.2f\n", time_load + time_check + time_size);

		// Success
		return;
	}
}
