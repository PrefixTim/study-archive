use super::feature::Featuer;

pub type Label = f64;
pub struct DataInstance {
    pub label: Label,
    pub features: Vec<Featuer>
}

pub type InstanceId = usize;
pub type InstanceArena = Vec<DataInstance>;