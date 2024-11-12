import pygame
import sys
import math

# Initialize Pygame
pygame.init()

# Constants
SCREEN_WIDTH = 1200
SCREEN_HEIGHT = 800
NODE_RADIUS = 20
LINE_COLOR = (200, 200, 200)
NODE_COLOR = (100, 149, 237)
HIGHLIGHT_COLOR = (255, 0, 0)
FONT_COLOR = (255, 255, 255)
BG_COLOR = (30, 30, 30)
FPS = 60

# Segment Tree Data Structure
class SegmentTree:
    def __init__(self, data):
        self.n = len(data)
        self.tree = [0] * (2 * self.n)
        self.ranges = [[0,0]] * (2 * self.n)
        self.build(data)
    
    def build(self, data):
        for i in range(self.n):
            self.tree[self.n + i] = data[i]
            self.ranges[self.n+i]=[i,i]
        for i in range(self.n - 1, 0, -1):
            self.tree[i] = self.tree[i * 2] + self.tree[i * 2 + 1]
            self.ranges[i] = [min(self.ranges[i * 2][0],self.ranges[i * 2 + 1][0]),max(self.ranges[i * 2][1],self.ranges[i * 2 + 1][1])]

    def update(self, pos, value):
        pos += self.n
        self.tree[pos] = value
        while pos > 1:
            pos //= 2
            self.tree[pos] = self.tree[pos * 2] + self.tree[pos * 2 + 1]

    def query(self, left, right):
        left += self.n
        right += self.n
        res = 0
        while left < right:
            if left % 2 == 1:
                res += self.tree[left]
                left += 1
            if right % 2 == 1:
                right -= 1
                res += self.tree[right]
            left //= 2
            right //= 2
        return res

# Tree Visualizer
class SegmentTreeVisualizer:
    def __init__(self, screen, segment_tree):
        self.screen = screen
        self.segment_tree = segment_tree
        self.font = pygame.font.SysFont(None, 24)
        self.nodes = []
        self.queries=[]

    def layout_nodes(self):
        # Position the nodes in the tree structure
        
        n = self.segment_tree.n
        self.nodes = [(-1,-1,-1) for i in range(2*n-1)]
        depth = math.ceil(math.log2(2 * n))
        for i in range(n, 2 * n):
            level = math.floor(math.log2(i))
            index_in_level = i - (1 << level)
            y = 100 + level * 100
            x = SCREEN_WIDTH // ((1 << (level))+1) * (1 + index_in_level)
            self.nodes[i-1]=(i, x, y)
        for i in range(n-1,0,-1):
            level = math.floor(math.log2(i))
            index_in_level = i - (1 << level)
            y = 100 + level * 100
            left=self.nodes[i*2-1][1]
            right=self.nodes[i*2][1]
            x=(left+right)/2
            self.nodes[i-1]=(i, x, y)

    def draw_tree(self):
        for i, (idx, x, y) in enumerate(self.nodes):
            value = self.segment_tree.tree[idx] if idx < len(self.segment_tree.tree) else 0
            l = self.segment_tree.ranges[idx][0] if idx < len(self.segment_tree.tree) else 0
            r = self.segment_tree.ranges[idx][1] if idx < len(self.segment_tree.tree) else 0
            # Draw lines to children
            left_child = idx * 2
            right_child = idx * 2 + 1
            if left_child < len(self.nodes)+1:
                pygame.draw.line(self.screen, LINE_COLOR, (x, y), (self.nodes[left_child - 1][1], self.nodes[left_child - 1][2]), 2)
            if right_child < len(self.nodes)+1:
                pygame.draw.line(self.screen, LINE_COLOR, (x, y), (self.nodes[right_child - 1][1], self.nodes[right_child - 1][2]), 2)
            pygame.draw.circle(self.screen, NODE_COLOR, (x, y), NODE_RADIUS)
            ad=""
            if l==r:
                ad="["+str(r)+"]"
            else:
                ad="["+str(l)+","+str(r)+"]"
            text = self.font.render(ad, True, FONT_COLOR)
            self.screen.blit(text, (x - text.get_width() // 2, y - text.get_height() // 2))
    def draw_query(self,l,r):
        None

    def highlight_node(self, idx,color=HIGHLIGHT_COLOR):
        _, x, y = self.nodes[idx - 1]
        pygame.draw.circle(self.screen, HIGHLIGHT_COLOR, (x, y), NODE_RADIUS, 3)

    def highlight_edge(self, start, end,color=HIGHLIGHT_COLOR):
        pygame.draw.line(self.screen, color, start, end, 3)

    def update_node_value(self, idx, new_value):
        self.segment_tree.update(idx - self.segment_tree.n, new_value)
        self.draw_tree()

    def animate_query(self, left, right):
        # Animate only the edges and nodes involved in the query
        nodes_to_visit = []
        left += self.segment_tree.n
        right += self.segment_tree.n

        while left >0:
            
            nodes_to_visit.append(left)
            
            nodes_to_visit.append(right)
            left //= 2
            right //= 2
        print(*nodes_to_visit)
        print(len(self.nodes))
        for node in nodes_to_visit:
            nx=len(nodes_to_visit)
            left_child = 2*node
            right_child = node*2 + 1
            if left_child-1<nx:
                print(1)
                self.highlight_edge(self.nodes[left_child - 1][1:], self.nodes[node - 1][1:],[0,255,0])
            if right_child-1<nx:
                print(1)
                self.highlight_edge(self.nodes[right_child - 1][1:], self.nodes[node - 1][1:],[0,255,0])
        for node in nodes_to_visit:
            if node > 1:
                parent = node // 2
                self.highlight_edge(self.nodes[parent - 1][1:], self.nodes[node - 1][1:])
            self.highlight_node(node)
        pygame.display.flip()

# Pygame main loop
def main():
    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    pygame.display.set_caption("Segment Tree Visualizer")
    clock = pygame.time.Clock()

    # Initialize the segment tree
    data = [1, 3, 5, 7, 9, 11,12,4,1, 3, 5, 7, 9, 11,12,4,1, 3, 5, 7, 9, 11,12,4,1, 3, 5, 7, 9, 11,12,4]
    segment_tree = SegmentTree(data)
    visualizer = SegmentTreeVisualizer(screen, segment_tree)
    visualizer.layout_nodes()
    

    running = True
    while running:
        screen.fill(BG_COLOR)
        visualizer.draw_tree()
        pygame.display.flip()
        l=int(input("Enter left range: "))
        r=int(input("Enter right range: "))
        visualizer.animate_query(l,r)        
        x=int(input())

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Simulate an update and query for visualization
        # Uncomment these lines one at a time to test animations
        # visualizer.update_node_value(3, 15)  # Update example
        # result = visualizer.animate_query(2, 5)  # Query example from index 2 to 5

        pygame.display.flip()
        clock.tick(FPS)

    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()
