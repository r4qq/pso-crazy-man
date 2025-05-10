
pub struct Point 
{
    position: Vec<f64>,
    velocityVector: Vec<f64>,
    personalBest: Vec<f64>,
    grade: f64,
}

impl Point 
{
    pub fn new(startPos: Vec<f64>, startVelocity: Vec<f64>) -> Self 
    {
        Point {
            position: startPos.clone(),
            velocityVector: startVelocity,
            personalBest: startPos,
            grade: f64::MAX,
        }
    }    

    pub fn evalPoint(&mut self, func: &dyn Fn(&[f64]) -> f64)
    {
        let currentGrade = func(&self.position);

        if currentGrade < self.grade 
        {
            self.personalBest = self.position.clone();
            self.grade = currentGrade;    
        }
    }

    pub fn updatePosition(&mut self)
    {
        for (pos, vel) in self.position.iter_mut().zip(self.velocityVector.iter())
        {
            *pos += *vel;
        }
    }

    pub fn updateVelocity(&mut self, alpha: f64, beta: f64, epsilon1: f64, epsilon2: f64, globalBest: &Vec<f64>)
    {
        for i in 0..self.velocityVector.len()
        {
            self.velocityVector[i] += alpha * epsilon1 * (globalBest[i] - self.position[i]) + 
                                      beta * epsilon2 * (self.personalBest[i] - self.position[i]);
        }
    }

    pub fn enforceBounds(&mut self, bounds: (f64, f64))
    {
        for i in 0..self.velocityVector.len()
        {
            self.position[i] = bounds.0.max(bounds.1.min(self.position[i]));
        }
    }
}  