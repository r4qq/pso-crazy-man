
use rand::{rng, Rng};
use rand_distr::{Distribution, Uniform};

use crate::point::{self, Point};

pub struct Pso<'a>
{
    alpha: f64,
    beta: f64,
    epoch: u64,
    pointsAmount: u64,
    pointsDimenions: u8,
    bounds: (f64, f64),
    funcToMinimize: &'a dyn Fn(Vec<f64>) -> f64, 
    globalBestPos: Vec<Point>,
    gloabalBestval: f64,
}

impl<'a> Pso<'a>
{
    pub fn new(
        startAlpha: f64,
        startBeta: f64,
        startEpoch: u64,
        startPointsAmount: u64,
        startPointsDimenions: u8,
        startBounds: (f64, f64),
        startFuncToMinimize: &'a dyn Fn(Vec<f64>) -> f64,
    ) -> Self {
        Pso { 
            alpha: startAlpha, 
            beta: startBeta, 
            epoch: startEpoch, 
            pointsAmount: startPointsAmount, 
            pointsDimenions: startPointsDimenions, 
            bounds: startBounds, 
            funcToMinimize: startFuncToMinimize, 
            globalBestPos: Vec::new(),
            gloabalBestval: f64::MAX,
        }
    }

    pub fn initPoints(&mut self)
    {
        let mut rng = rng(); 
        let distrPos = Uniform::new(self.bounds.0, self.bounds.1).expect("Invalid bounds: lower must be less than upper");
        let distrVel = Uniform::new(-1.0, 1.0);

        for _ in 0..self.pointsAmount {
            let mut start_pos = Vec::with_capacity(self.pointsDimenions.into());
            let mut velocity_vector = Vec::with_capacity(self.pointsDimenions.into());
            
            for _ in 0..self.pointsDimenions {
                start_pos.push(rng.sample(&distrPos)); 
                velocity_vector.push(rng.sample(&distrVel));
            }
            
            self.points.push(start_pos);
            self.velocities.push(velocity_vector);
        }
    }
    
}