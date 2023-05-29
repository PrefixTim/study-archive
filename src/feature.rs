pub type Featuer = usize;

#[derive(Debug, Clone)]
pub struct FeatureSet {
    feat_state: Vec<bool>,
}

impl FeatureSet {
    pub fn new_full(size: usize) -> Self {
        Self {
            feat_state: vec![true; size],
        }
    }
    pub fn new_empty(size: usize) -> Self {
        Self {
            feat_state: vec![false; size],
        }
    }

    // fn is_feat_enbl(&self, feat: Featuer) -> bool {
    //     self.feat_state[feat as usize]
    // }

    pub fn set_feat(&mut self, feat: &Featuer, state: bool) {
        self.feat_state[*feat as usize] = state;
    }

    pub fn set_feat_clone(&mut self, feat: &Featuer, state: bool) -> Self {
        let mut node = self.clone();

        node.feat_state[*feat as usize] = state;
        node
    }

    pub fn is_empty(&self) -> bool {
        !self.feat_state.contains(&true)
    }

    pub fn get_features(&self) -> Vec<Featuer> {
        self.feat_state
            .iter()
            .enumerate()
            .filter_map(|(i, e)| {
                if *e {
                    Some(i)
                } else {
                    {
                        None
                    }
                }
            })
            .collect()
    }
}