#
# Python script for parsing Wycheproof test vectors for ECDH
# for example python3 parsex.py x25519_test.json
# Output should be C digestible, outputting on separate lines the comment, the public key, the private key, the shared secret and finally the outcome, valid or invalid
#

import sys

# extract substring at position ptr, return substring and updated ptr
def extract(s,ptr,id) :
    pk=s.find(id,ptr)
    fpk=s.find('"',pk+len(id))  # skip "
    lpk=s.find('"',fpk+1)+1
    return s[fpk+1:lpk-1],lpk

inputfile=sys.argv[1]

# read whole file into string
with open(inputfile) as f:     # Wycheproof JSON test vectors
    data = f.read()

ptr=0
finished=False

while not finished :
    ss,ptr=extract(data,ptr,'"comment"') # ptr points to public key. 
    print(ss)
    ss,ptr=extract(data,ptr,'"public"')
    print(ss)
    ss,ptr=extract(data,ptr,'"private"')
    print(ss)
    ss,ptr=extract(data,ptr,'"shared"')
    print(ss)
    ss,ptr=extract(data,ptr,'"result"')
    print(ss)
    ncm=data.find('"comment"',ptr)
    if ncm<0:    # there is no next comment, we are done
        finished=True
