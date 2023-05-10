pub mod np_node;
pub mod np_problem;

#[cfg(test)]
mod tests {
    use crate::problems::problem_trait::Problem;

    use super::{
        np_node::{euclidean_distance_heuristic, misplaced_tile_heuristic, zero_heuristic},
        np_problem::Npuzle,
    };

    fn get_init_states() -> Vec<(Vec<i64>, String)> {
        vec![
            (vec![1, 2, 3, 4, 8, 0, 7, 6, 5], "Ex 1".to_owned()),
            (vec![1, 0, 3, 4, 2, 6, 7, 5, 8], "Ex 2".to_owned()),
            (vec![1, 2, 3, 4, 5, 6, 7, 8, 0], "Trival".to_owned()),
            (vec![1, 2, 0, 4, 5, 3, 7, 8, 6], "Easy".to_owned()),
            (vec![8, 7, 1, 6, 0, 2, 5, 4, 3], "Oh Boy".to_owned()),
            (vec![1, 2, 3, 4, 5, 6, 7, 0, 8], "Very Easy".to_owned()),
            (vec![0, 1, 2, 4, 5, 3, 7, 8, 6], "Doable".to_owned()),
            (vec![0, 2, 8, 5, 6, 4, 3, 1, 7], "My test 1".to_owned()),
            (vec![6, 0, 4, 2, 3, 1, 7, 8, 5], "My test 2".to_owned()),
            (vec![8, 0, 2, 3, 5, 6, 1, 7, 4], "My test 3".to_owned()),
            (vec![1, 3, 6, 5, 0, 2, 8, 4, 7], "My test 4".to_owned()),
            (vec![1, 2, 3, 0, 5, 6, 8, 4, 7], "My test 5".to_owned()),
            // (vec![1, 2, 3, 4, 5, 6, 8, 7, 0], "".to_owned()),
        ]
    }
    #[test]
    fn general_search() -> Result<(), ()> {
        get_init_states()
            .iter()
            .map(|(state, name)| {
                (
                    Npuzle::new(state.clone(), zero_heuristic, false)
                        .unwrap()
                        .solve(),
                    name,
                )
            })
            .for_each(|(res, name)| {
                assert!(res.is_ok(), "Should be able to Solve {name}");
                let stat = res.unwrap().stats;
                print!(
                    "{}: {}, {}, {}\n",
                    name, stat.expanded, stat.max_queue, stat.goal_depth
                )
            });
        Ok(())
    }
    #[test]
    fn astar_misplaced() -> Result<(), ()> {
        get_init_states()
            .iter()
            .map(|(state, name)| {
                (
                    Npuzle::new(state.clone(), misplaced_tile_heuristic, false)
                        .unwrap()
                        .solve(),
                    name,
                )
            })
            .for_each(|(res, name)| {
                assert!(res.is_ok(), "Should be able to Solve {name}");
                let stat = res.unwrap().stats;
                print!(
                    "{}: {}, {}, {}\n",
                    name, stat.expanded, stat.max_queue, stat.goal_depth
                )
            });
        Ok(())
    }
    #[test]
    fn astar_euclid() -> Result<(), ()> {
        get_init_states()
            .iter()
            .map(|(state, name)| {
                (
                    Npuzle::new(state.clone(), euclidean_distance_heuristic, false)
                        .unwrap()
                        .solve(),
                    name,
                )
            })
            .for_each(|(res, name)| {
                assert!(res.is_ok(), "Should be able to Solve {name}");
                let stat = res.unwrap().stats;
                print!(
                    "{}: {}, {}, {}\n",
                    name, stat.expanded, stat.max_queue, stat.goal_depth
                )
            });
        Ok(())
    }
}
