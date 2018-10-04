import os
def test_eq(eq):
	
	f = os.popen('bin/sm_calc "'+eq+'"')
	test_result = f.read()
	expr=eq.replace(" ", "")
	expr=eq.replace(",", ".")
	
	tt=eval(expr)
	true_result = round(tt,2)
	
	if float(test_result) == true_result:
		print("OK")
	else:
		print("Failed for: "+eq)




test_eq(' 2 + 2 ')
test_eq(' - 1 + 5 - 3 ')
test_eq(' -10 + (8 * 2.5) - (3 / 1,5) ')
test_eq('1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)')

test_eq('- (1 + (2 * (2.5 + 2.5 + (3 - 2))))/10 - (3 / 1.5)')
