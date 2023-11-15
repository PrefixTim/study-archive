"""
Custom Topology:

    +--------+   +--------+
    | client |---| server |
    +--------+   +--------+

"""

from mininet.topo import Topo

class MyTopo(Topo):
    def build(self):
        # Add hosts
        client = self.addHost('client')
        router = self.addHost('router')
        server = self.addHost('server')

	# Add Link
        self.addLink(client, router)
        self.addLink(router, server)

topos = {'mytopo': (lambda: MyTopo())}
