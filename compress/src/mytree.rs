use std::usize;

type Myptr = Option<usize>;
enum Node<T> {
    Leaf(Option<T>),
    Node((usize, usize))
}
// No remove
struct MyTree<T> {
    nodes: Vec<(Option<T>, usize, usize)>,
    head: Myptr,
}

impl<T> MyTree<T> {
    pub fn new() -> MyTree<T> {
        Self{nodes: Vec::new(), head: None}
    }

    pub fn get_head(&self) -> Myptr {
        self.head
    }

    pub fn get(&self, ptr: Myptr) -> Option<&T> {
        if let Some(p) = ptr {
            Some(&self.nodes[p])
        } else {
            None
        }
    }

    pub fn insert_leaf(&mut self, val: T) -> usize {
        self.nodes.push((Some(val), None, None));
        self.nodes.len() - 1
    }

    pub fn connect(&mut self, ptrl: usize, ptrr: usize) {
        self.nodes.push((ptrl, ptrr));

    }
}




//     0
//   /  \
//  1    2 
// /
//3