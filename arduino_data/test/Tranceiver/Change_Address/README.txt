This code gives each node its own address, then simply opens a different reading pipe to switch to a different address. The MAIN ONLY READS
in this code, and the NODE ONLY WRITES. The main node listens one by one.

PROS:
- Can easily get nodes in order (1, 2, 3, 4, ...), and can take note if one doesn't send a message easily. Good for debugging

CONS: 
- It doesn't really allow for commands to be sent out, which isn't ideal.
- (In current state 7/19/2021) code just stops if a node doesn't respond. 
- Node end isn't designed to listen for commands