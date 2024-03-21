# WordChecker: A System for Validating Letter Correspondence

## General Information
- **Project Name**: WordChecker
- **Author**: Amadori Dario
- **Academic Year**: 2022
- **Course**: Algorithms and Principles of Computer Science
- **Institution**: Politecnico di Milano
- **Date**: 2022

## Project Description
The WordChecker project for the academic year 2022 aims to develop a system capable of validating the correspondence between the letters of two words of the same length. This system is designed to work with words composed of lowercase and uppercase alphabetical characters (a-z, A-Z), numeric digits (0-9), and the symbols '-' (dash) and '_' (underscore).

## Key Features
- **Letter Correspondence Validation**: WordChecker verifies the match between letters of words of equal length.
- **Manageable and Expandable Set of Admissible Words**: The system can dynamically manage and expand a set of admissible words.
- **Processing of Comparison Sequences**: WordChecker processes sequences of "matches", comparing reference words with a series of words to be compared.
- **Dynamic Addition of Admissible Words**: New admissible words can be dynamically added to the system through specific commands.

## How It Works
WordChecker reads from standard input a series of instructions and data, including a value 'k' representing the length of the words to be processed, and an arbitrary sequence of admissible words of length 'k', ensuring no duplicates. After the input of admissible words, the system reads sequences of "matches", indicated by the '+new_match' command, involving a reference word of length 'k' and a sequence of words to compare against the reference word.

## Key Concepts
- Validates letter correspondence between words of equal length.
- Manages an expandable set of admissible words.
- Processes sequences of matches with reference and comparison words.
- Allows for the dynamic addition of admissible words to the system.

This project represents the final examination for the course "Algorithms and Principles of Computer Science" at the Computer Engineering program of the Politecnico di Milano.
