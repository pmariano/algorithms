# Given a decimal (num) give the correponding hex. eg: 88736 -> 15AA0
#
hex = {
	'10'=>'A',
	'11'=>'B',
	'12'=>'C',
	'13'=>'D',
	'14'=>'E',
	'15'=>'F'
}

num = 776353
str = ""

while num > 0
	res = (num % 16).to_s
	str = "#{hex[res] || res}#{str}"

	num /= 16
end

puts "GOT #{str}"

