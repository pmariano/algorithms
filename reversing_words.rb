#Give an array of characters that result a sentence, write a method that will print the words in reverse order (not the characters). Watch out for punctuation marks. An example input would be: "These puzzlers get easier every week! Wonder if there's a catch?"

input = %w(These puzzlers get easier every week! Wonder if there's a catch?)
p input

def revert_word(word)
	(word.length / 2).times do |i|
		w = word[i]
		word[i] = word[(word.length - i - 1)]
		word[word.length - i - 1] = w
	end

	word
end

input.map! do |e|
	revert_word e
end

p input.join " "
